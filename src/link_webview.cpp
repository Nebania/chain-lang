#include "link_webview.h"
#include "webview.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>

// Mendeteksi sistem operasi Windows secara kompilasi
#if defined(_WIN32) || defined(_WIN64)
    #define IS_WINDOWS 1
#else
    #define IS_WINDOWS 0
#endif

namespace SysWebview {

    static webview::webview* currentWebview = nullptr;

    void create(const std::string& title, int width, int height, const std::string& html) {
        try {
            std::filesystem::path tempPath = std::filesystem::current_path() / ".chain_temp_ui.html";
            
            std::ofstream out(tempPath);
            if (out.is_open()) {
                out << html;
                out.close();
            }

            std::string fileUrl;
            if (IS_WINDOWS) {
                fileUrl = "file:///" + tempPath.string();
                for (auto& c : fileUrl) {
                    if (c == '\\') c = '/';
                }
            } else {
                fileUrl = "file://" + tempPath.string();
            }

            webview::webview w(false, nullptr);
            currentWebview = &w; 
            
            w.set_title(title);
            w.set_size(width, height, WEBVIEW_HINT_NONE);
            w.navigate(fileUrl);

            w.bind("chain_print", [](std::string s) -> std::string {

                if (s.length() >= 4 && s.front() == '[' && s.back() == ']') {
                    s = s.substr(2, s.length() - 4);
                }
                std::cout << "\033[1;36m[Webview UI]\033[0m " << s << std::endl;
                return ""; 
            });

            w.run();

            currentWebview = nullptr; 
            std::filesystem::remove(tempPath);

        } catch (const std::exception& e) {
            std::cerr << "[Webview Fatal Error] " << e.what() << std::endl;
            currentWebview = nullptr;
        }
    }

    void eval(const std::string& js) {
        if (currentWebview != nullptr) {
            currentWebview->eval(js);
        }
    }
}
