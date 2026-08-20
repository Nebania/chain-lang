#pragma once
#include <string>

namespace SysWebview {
    void create(const std::string& title, int width, int height, const std::string& html);
    void eval(const std::string& js);
}