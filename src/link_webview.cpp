#include "link_webview.h"
#include "webview.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <vector>
#include <mutex>

 
std::string urlEncode(const std::string& value) {
    std::string escaped;
    for (char c : value) {
        if (isalnum((unsigned char)c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped += c;
        } else {
            char buf[4];
            snprintf(buf, sizeof(buf), "%%%02X", (unsigned char)c);
            escaped += buf;
        }
    }
    return escaped;
}

namespace SysWebview {
    
    static std::unique_ptr<webview::webview> currentWebview = nullptr;
 
    static std::vector<std::string> jsQueue;
    static std::mutex queueMutex;

    void create(const std::string& title, int width, int height, const std::string& html) {
        try {
            std::string tempFilePath = std::filesystem::current_path().string() + "/.chain_temp_ui.html";
 
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                jsQueue.clear();
            }
 
            std::string injectedHtml = html + R"(
                <script>
                    setInterval(function() {
                        if (window.chain_poll) {
                            chain_poll('');
                        }
                    }, 50);
                </script>
            )";

            std::ofstream out(tempFilePath);
            if (out.is_open()) {
                out << injectedHtml;
                out.close();
            } else {
                std::cout << "[Webview Error] Gagal membuat file UI sementara." << std::endl;
                return;
            }

            currentWebview = std::make_unique<webview::webview>(false, nullptr);
            currentWebview->set_title(title);
            currentWebview->set_size(width, height, WEBVIEW_HINT_NONE);
            currentWebview->navigate("file://" + tempFilePath);
 
            currentWebview->bind("chain_print", [](std::string s) -> std::string {
                if (s.length() >= 4) s = s.substr(2, s.length() - 4);
                std::cout << "\033[1;36m[Webview UI]\033[0m " << s << std::endl;
                return ""; 
            });
 
            currentWebview->bind("chain_poll", [](std::string s) -> std::string {
                std::lock_guard<std::mutex> lock(queueMutex);
                if (currentWebview != nullptr) {
     
                    for (const std::string& jsCode : jsQueue) {
                        currentWebview->eval(jsCode);
                    }
                }
                jsQueue.clear();  
                return ""; 
            });

            currentWebview->run();
            
            currentWebview = nullptr; 
            std::filesystem::remove(tempFilePath);

        } catch (const std::exception& e) {
            std::cout << "[Webview Fatal Error] " << e.what() << std::endl;
            currentWebview = nullptr; 
        }
    }

    void eval(const std::string& js) {
        std::lock_guard<std::mutex> lock(queueMutex);
        jsQueue.push_back(js);
    }
}
