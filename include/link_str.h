// std_string.h
#pragma once
#include <string>
#include <vector>

namespace SysString {
    std::string trim(const std::string& str);
    std::string replace(std::string str, const std::string& from, const std::string& to);
    std::vector<std::string> split(std::string str, const std::string& delimiter);
    std::string merge(const std::vector<std::string>& list, const std::string& delimiter);
    
    std::string substring(const std::string& str, int start, int length);
    std::string toLower(const std::string& str);
    std::string toUpper(const std::string& str);
    std::string pop(const std::string& input);
}
