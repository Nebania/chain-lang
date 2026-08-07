#pragma once
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <dlfcn.h>

class LinkWrapper {
public:
    static void execute(const std::string& code, Runtime* rt) {
        std::string hashName = "mod_" + std::to_string(code.length()); // Simply use length for now
        std::string cacheDir = ".link_cache/";
        std::string cppPath = cacheDir + hashName + ".cpp";
        std::string soPath = cacheDir + hashName + ".so";

        // 1. Caching Check (Timestamp + Size)
        if (!needsRecompile(cppPath, soPath)) {
            runSharedObject(soPath);
            return;
        }

        // 2. Magic Injection & Template Generation
        std::ofstream out(cppPath);
        out << "#include <iostream>\n"
            << "#define print(x) std::cout << x << std::endl;\n"
            << "extern \"C\" void link_entry() {\n"
            << code // User code is inserted here
            << "\n}";
        out.close();

        // 3. Compile via g++
        std::string cmd = "g++ -shared -fPIC -o " + soPath + " " + cppPath;
        if (system(cmd.c_str()) != 0) return;

        runSharedObject(soPath);
    }

private:
    static bool needsRecompile(const std::string& cpp, const std::string& so) {
        struct stat st_cpp, st_so;
        if (stat(cpp.c_str(), &st_cpp) != 0 || stat(so.c_str(), &st_so) != 0) return true;
        return st_cpp.st_mtime > st_so.st_mtime;
    }

    static void runSharedObject(const std::string& path) {
        pid_t pid = fork();
        if (pid == 0) { // Child: isolate
            void* handle = dlopen(path.c_str(), RTLD_NOW);
            if (handle) {
                auto func = (void(*)())dlsym(handle, "link_entry");
                if (func) func();
                dlclose(handle);
            }
            exit(0);
        } else {
            waitpid(pid, nullptr, 0); // Parent: keep Chain safe
        }
    }
};