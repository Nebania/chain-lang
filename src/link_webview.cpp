#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>
#include "link_webview.h"
#include "webview.h"

#if defined(_WIN32) || defined(_WIN64)
    #define IS_WINDOWS 1
#elif defined(__APPLE__) || defined(__MACH__)
    #define IS_MACOS 1
#elif defined(__linux__)
    #define IS_LINUX 1
#endif

namespace {
    std::unique_ptr<webview::webview> currentWebview = nullptr;
}

namespace SysWebview {

void create(const std::string& title, int width, int height, const std::string& html) {
    try {
        std::string tempFilePath = (std::filesystem::current_path() / ".chain_temp_ui.html").string();
        
        std::ofstream out(tempFilePath);
        if (out.is_open()) {
            out << html;
            out.close();
        }

        // ----------------------------------------------------
        // WINDOWS (Edge/WebView2)
        // ----------------------------------------------------
        #if defined(IS_WINDOWS)
            currentWebview = std::make_unique<webview::webview>(true, nullptr);

        // ----------------------------------------------------
        // LINUX (GTK3 + WebKitGtk)
        // ----------------------------------------------------
        #elif defined(IS_LINUX)
            currentWebview = std::make_unique<webview::webview>(false, nullptr);

        // ----------------------------------------------------
        // macOS (Cocoa / WebKit)
        // ----------------------------------------------------
        #elif defined(IS_MACOS)
            currentWebview = std::make_unique<webview::webview>(false, nullptr);

        // ----------------------------------------------------
        // FALLBACK FOR OTHER ARCHITECTURES
        // ----------------------------------------------------
        #else
            currentWebview = std::make_unique<webview::webview>(false, nullptr);
        #endif

        if (!currentWebview) return;

        currentWebview->set_title(title);
        currentWebview->set_size(width, height, WEBVIEW_HINT_NONE);
        
        #if defined(IS_WINDOWS)
            currentWebview->navigate("file:///" + tempFilePath); // Windows 3 slashes
        #else
            currentWebview->navigate("file://" + tempFilePath);  // POSIX (Linux/macOS)
        #endif

        currentWebview->bind("chain_print", [](std::string s) -> std::string {
            if (s.length() >= 4) s = s.substr(2, s.length() - 4);
            std::cout << "\033[1;36m[Webview UI]\033[0m " << s << std::endl;
            return ""; 
        });

        currentWebview->run();

        currentWebview.reset(); 
        std::filesystem::remove(tempFilePath);

    } catch (const std::exception& e) {
        std::cout << "[Webview Fatal Error] " << e.what() << std::endl;
        currentWebview.reset();
    }
}

void eval(const std::string& js) {
    if (currentWebview) {
        currentWebview->eval(js);
    }
}

} 
