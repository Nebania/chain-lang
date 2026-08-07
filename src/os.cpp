#include "os.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <array>

namespace Sys {

    std::string readFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) return "";
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    void writeFile(const std::string& path, const std::string& content, bool append) {
        std::ofstream file;
        if (append) file.open(path, std::ios::app);
        else file.open(path);
        if (file.is_open()) file << content;
    }

    bool fileExists(const std::string& path) {
        std::ifstream file(path);
        return file.good();
    }

    void removeFile(const std::string& path) {
        std::remove(path.c_str());
    }

    // Fix Warning unique_ptr dengan custom deleter
    struct PipeCloser {
        void operator()(FILE* file) const {
            if (file) {
                #ifdef _WIN32
                _pclose(file);
                #else
                pclose(file);
                #endif
            }
        }
    };

    std::string exec(const char* cmd) {
        std::array<char, 128> buffer;
        std::string result;

        #ifdef _WIN32
            std::unique_ptr<FILE, PipeCloser> pipe(_popen(cmd, "r"));
        #else
            std::unique_ptr<FILE, PipeCloser> pipe(popen(cmd, "r"));
        #endif

        if (!pipe) return "";
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

    bool contains(const std::string& haystack, const std::string& needle) {
        return haystack.find(needle) != std::string::npos;
    }

    std::string unescape(const std::string& s) {
        std::string res = "";
        for (size_t i = 0; i < s.length(); i++) {
            if (s[i] == '\\' && i + 1 < s.length()) {
                switch (s[i + 1]) {
                    case 'n': res += '\n'; i++; break;
                    case 't': res += '\t'; i++; break;
                    case '\\': res += '\\'; i++; break;
                    case '"': res += '"'; i++; break;
                    default: res += s[i]; break; 
                }
            } else {
                res += s[i];
            }
        }
        return res;
    }
    std::string getEnv(const std::string& key) {
        char* val = std::getenv(key.c_str());
        return val ? std::string(val) : "";
    }

    void setEnv(const std::string& key, const std::string& value) {
        #ifdef _WIN32
            std::string envString = key + "=" + value;
            _putenv(envString.c_str());
        #else
            setenv(key.c_str(), value.c_str(), 1); 
        #endif
    }
}
