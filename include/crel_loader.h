#pragma once

#include <string>
#include <CREL/CREL.hpp>

class LibraryLoader {
public:
    static bool load(const std::string& path, CREL& api);
};