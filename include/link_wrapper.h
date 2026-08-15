#pragma once

#include <string>
#include <fstream>
#include <cstdlib>
#include <sys/stat.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <sys/wait.h>
    #include <dlfcn.h>
#endif

class LinkWrapper {
public:
    static void execute(const std::string& code, Runtime* rt) {
        std::string hashName = "mod_" + std::to_string(code.length());

        std::string cacheDir = ".link_cache/";
        std::string cppPath = cacheDir + hashName + ".cpp";

#ifdef _WIN32
        std::string libPath = cacheDir + hashName + ".dll";
#else
        std::string libPath = cacheDir + hashName + ".so";
#endif

        // Create cache directory
#ifdef _WIN32
        CreateDirectoryA(cacheDir.c_str(), nullptr);
#else
        mkdir(cacheDir.c_str(), 0755);
#endif

        // Check cache
        if (!needsRecompile(cppPath, libPath)) {
            runSharedObject(libPath);
            return;
        }

        // Generate C++
        std::ofstream out(cppPath);

        out << "#include <iostream>\n"
            << "#define print(x) std::cout << x << std::endl;\n"
            << "extern \"C\" void link_entry() {\n"
            << code
            << "\n}";

        out.close();

        // Compile
#ifdef _WIN32
        std::string cmd =
            "g++ -shared -o \"" +
            libPath + "\" \"" +
            cppPath + "\"";
#else
        std::string cmd =
            "g++ -shared -fPIC -o \"" +
            libPath + "\" \"" +
            cppPath + "\"";
#endif

        if (system(cmd.c_str()) != 0)
            return;

        runSharedObject(libPath);
    }

private:

    static bool needsRecompile(
        const std::string& cpp,
        const std::string& lib
    ) {
        struct stat st_cpp{};
        struct stat st_lib{};

        if (stat(cpp.c_str(), &st_cpp) != 0)
            return true;

        if (stat(lib.c_str(), &st_lib) != 0)
            return true;

        return st_cpp.st_mtime > st_lib.st_mtime;
    }


#ifdef _WIN32

    // Windows DLL loading
    static void runSharedObject(const std::string& path) {
        HMODULE handle = LoadLibraryA(path.c_str());

        if (!handle)
            return;

        using LinkEntry = void(*)();

        LinkEntry func =
            reinterpret_cast<LinkEntry>(
                GetProcAddress(handle, "link_entry")
            );

        if (func)
            func();

        FreeLibrary(handle);
    }

#else

    // Linux .so loading + process isolation
    static void runSharedObject(const std::string& path) {
        pid_t pid = fork();

        if (pid == 0) {
            void* handle = dlopen(
                path.c_str(),
                RTLD_NOW
            );

            if (handle) {
                using LinkEntry = void(*)();

                LinkEntry func =
                    reinterpret_cast<LinkEntry>(
                        dlsym(handle, "link_entry")
                    );

                if (func)
                    func();

                dlclose(handle);
            }

            _exit(0);
        }

        if (pid > 0) {
            waitpid(pid, nullptr, 0);
        }
    }

#endif
};