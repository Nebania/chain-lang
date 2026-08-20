#pragma once
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

class ReplEditor {
private:
    std::vector<std::string> history;
    int historyIdx = -1;

    void clearCurrentLine(int len) {
        std::cout << "\r";  
        for(int i = 0; i < len + 10; i++) std::cout << " "; 
        std::cout << "\r";  
    }

public:
    void addToHistory(const std::string& line) {
        if (line.empty()) return;
        if (!history.empty() && history.back() == line) return; 
        history.push_back(line);
    }

    std::string readLine(const std::string& prompt, int currentIndent = 0) {
        std::string buffer = "";
        int cursor = 0;  
        historyIdx = history.size();

        std::string fullPrompt = prompt;
        for(int i = 0; i < currentIndent; i++) fullPrompt += "  ";
        std::cout << fullPrompt << std::flush;

// --- MULAI RAW MODE (Aman) ---
#ifndef _WIN32
        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
#endif

        while (true) {
            int c;
#ifdef _WIN32
            c = _getch();
            if (c == 0 || c == 224) {
                int key = _getch();
                if (key == 75) { // Panah Kiri
                    if (cursor > 0) { cursor--; std::cout << "\b" << std::flush; }
                } 
                else if (key == 77) { // Panah Kanan
                    if (cursor < (int)buffer.length()) { std::cout << buffer[cursor] << std::flush; cursor++; }
                }
                else if (key == 72 || key == 80) { // Panah Atas & Bawah
                    if (key == 72 && historyIdx > 0) historyIdx--;
                    else if (key == 80) {
                        if (historyIdx < (int)history.size() - 1) historyIdx++;
                        else historyIdx = history.size();
                    } else continue; 
    
                    clearCurrentLine(fullPrompt.length() + buffer.length());
                    buffer = (historyIdx < (int)history.size()) ? history[historyIdx] : "";
                    std::cout << fullPrompt << buffer << std::flush;
                    cursor = buffer.length();
                }
                else if (key == 83) { // Tombol Delete
                    if (cursor < (int)buffer.length()) {
                        buffer.erase(cursor, 1);
                        std::cout << buffer.substr(cursor) << " \b";
                        for (size_t i = 0; i < buffer.length() - cursor; i++) std::cout << "\b";
                        std::cout << std::flush;
                    }
                }
                continue;
            }
#else
            c = getchar();
            if (c == 27) { // ANSI Escape Sequence (Linux/macOS)
                int c1 = getchar();
                if (c1 == '[') {
                    int c2 = getchar();
                    if (c2 == 'A') { // Panah Atas
                        if (historyIdx > 0) historyIdx--;
                        else continue;

                        clearCurrentLine(fullPrompt.length() + buffer.length());
                        buffer = history[historyIdx];
                        std::cout << fullPrompt << buffer << std::flush;
                        cursor = buffer.length();
                    }
                    else if (c2 == 'B') { // Panah Bawah
                        if (historyIdx < (int)history.size() - 1) historyIdx++;
                        else historyIdx = history.size();

                        clearCurrentLine(fullPrompt.length() + buffer.length());
                        buffer = (historyIdx < (int)history.size()) ? history[historyIdx] : "";
                        std::cout << fullPrompt << buffer << std::flush;
                        cursor = buffer.length();
                    }
                    else if (c2 == 'C') { // Panah Kanan
                        if (cursor < (int)buffer.length()) { std::cout << "\033[C" << std::flush; cursor++; }
                    }
                    else if (c2 == 'D') { // Panah Kiri
                        if (cursor > 0) { cursor--; std::cout << "\033[D" << std::flush; }
                    }
                    else if (c2 == '3') { // Tombol Delete
                        if (getchar() == '~') { 
                            if (cursor < (int)buffer.length()) {
                                buffer.erase(cursor, 1);
                                std::cout << buffer.substr(cursor) << " \b";
                                for (size_t i = 0; i < buffer.length() - cursor; i++) std::cout << "\b";
                                std::cout << std::flush;
                            }
                        }
                    }
                }
                continue;
            }
#endif
            
            if (c == 3 || c == 4) { 
#ifndef _WIN32
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt); 
#endif
                std::cout << "\n";
                if (c == 3) exit(0); 
                return "exit";
            }

            if (c == '\n' || c == '\r') {
                std::cout << "\n";
#ifndef _WIN32
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt); 
#endif
                return buffer;
            }

            if (c == 8 || c == 127) {
                if (cursor > 0) { 
                    cursor--;
                    buffer.erase(cursor, 1); 
                    std::cout << "\b" << buffer.substr(cursor) << " \b";
                    for (size_t i = 0; i < buffer.length() - cursor; i++) std::cout << "\b";
                    std::cout << std::flush;
                }
                continue;
            }

            if (c >= 32 && c <= 126) {
                buffer.insert(cursor, 1, (char)c);
                std::string remaining = buffer.substr(cursor);
                std::cout << remaining;
                for (size_t i = 0; i < remaining.length() - 1; i++) std::cout << "\b";
                cursor++;
                std::cout << std::flush;
            }
        }
    }
};