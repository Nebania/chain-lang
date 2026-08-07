#pragma once
#include <string>

namespace Sys {
    std::string readFile(const std::string& path);
    void writeFile(const std::string& path, const std::string& content, bool append);
    bool fileExists(const std::string& path);
    void removeFile(const std::string& path);
    std::string exec(const char* cmd);
    bool contains(const std::string& haystack, const std::string& needle);
    std::string unescape(const std::string& s);
    std::string getEnv(const std::string& key);
    void setEnv(const std::string& key, const std::string& value);
}
