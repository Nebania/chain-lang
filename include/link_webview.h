#pragma once
#include <string>

#if defined(_WIN32) || defined(_WIN64)
    #ifdef BUILDING_WEBVIEW_DLL
        #define WEBVIEW_API __declspec(dllexport)
    #else
        #define WEBVIEW_API __declspec(dllimport)
    #endif
#else
    #define WEBVIEW_API
#endif

namespace SysWebview {
    WEBVIEW_API void create(const std::string& title, int width, int height, const std::string& html);
    
}

