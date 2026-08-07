// std_string.cpp
#include "link_str.h"
#include <algorithm>
#include <cctype> 

namespace SysString {
	
	std::string pop(const std::string& input) {
        if (input.empty()) return ""; 
        
        std::string result = input;
        result.pop_back(); 
        return result;
    }

    std::string trim(const std::string& str) {
        const std::string whitespace = " \t\n\r";
        size_t first = str.find_first_not_of(whitespace);
        if (std::string::npos == first) return "";
        size_t last = str.find_last_not_of(whitespace);
        return str.substr(first, (last - first + 1));
    }

    std::string replace(std::string str, const std::string& from, const std::string& to) {
        if (from.empty()) return str;
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return str;
    }

    std::vector<std::string> split(std::string str, const std::string& delimiter) {
        std::vector<std::string> list;
        size_t pos = 0;
        std::string token;
        while ((pos = str.find(delimiter)) != std::string::npos) {
            token = str.substr(0, pos);
            list.push_back(token);
            str.erase(0, pos + delimiter.length());
        }
        list.push_back(str); // Sisa string terakhir
        return list;
    }

    std::string merge(const std::vector<std::string>& list, const std::string& delimiter) {
        std::string result = "";
        for (size_t i = 0; i < list.size(); ++i) {
            result += list[i];
            if (i < list.size() - 1) result += delimiter;
        }
        return result;
    }
    
    std::string substring(const std::string& str, int start, int length) {
        if (start < 0) return "";
        if (start >= (int)str.length()) return "";
        return str.substr(start, length);
    }
    
    std::string toLower(const std::string& str) {
        std::string result = str;
        // Iterate and transform each char
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        return result;
    }
    
    std::string toUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c){ return std::toupper(c); });
        return result;
    }
}
