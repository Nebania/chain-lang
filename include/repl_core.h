#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
    #define KEY_UP 72
    #define KEY_DOWN 80
    #define KEY_LEFT 75
    #define KEY_RIGHT 77
    #define KEY_ENTER 13
    #define KEY_BACKSPACE 8
#else
    #include <termios.h>
    #include <unistd.h>
    #define KEY_ENTER 10
    #define KEY_BACKSPACE 127
#endif

class ReplEditor {
private:
    std::vector<std::string> history;
    int historyIdx = -1;
    void clearCurrentLine(int len) {
        std::cout << "\r";  
        for(int i=0; i<len + 10; i++) std::cout << " "; 
        std::cout << "\r";  
    }

    int readChar() {
        #ifdef _WIN32
            return _getch();
        #else
            struct termios oldt, newt;
            int ch;
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
            ch = getchar();
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            return ch;
        #endif
    }

public:
    void addToHistory(const std::string& line) {
        if (line.empty()) return;
        if (!history.empty() && history.back() == line) return; // Do not duplicate
        history.push_back(line);
    }
    std::string readLine(const std::string& prompt, int currentIndent = 0) {
    std::string buffer = "";
    int cursor = 0;  
    historyIdx = history.size();

    std::string fullPrompt = prompt;
    for(int i=0; i<currentIndent; i++) fullPrompt += "  ";
    std::cout << fullPrompt << std::flush;

    while (true) {
        int c = readChar();
        #ifdef _WIN32
        if (c == 0 || c == 224) {
            int key = _getch();
            if (key == KEY_LEFT) {
                if (cursor > 0) { cursor--; std::cout << "\b" << std::flush; }
            } 
            else if (key == KEY_RIGHT) {
                if (cursor < (int)buffer.length()) { std::cout << buffer[cursor] << std::flush; cursor++; }
            }
            else if (key == KEY_UP || key == KEY_DOWN) {
                if (key == KEY_UP && historyIdx > 0) {
						historyIdx--;
					} else if (key == KEY_DOWN) {
						if (historyIdx < (int)history.size() - 1) historyIdx++;
						else historyIdx = history.size();
					} else {
						continue; 
					}
	
					clearCurrentLine(prompt.length() + buffer.length());
					buffer = (historyIdx < (int)history.size()) ? history[historyIdx] : "";
					std::cout << prompt << buffer << std::flush;
					cursor = buffer.length();
            }
            continue;
        }
        #else
        if (c == 27) {
             if (readChar() == '[') {
					int key = readChar();
					if (key == 'A' || key == 'B') {  
						if (key == 'A' && historyIdx > 0) {
							historyIdx--;
						} else if (key == 'B') {
							if (historyIdx < (int)history.size() - 1) historyIdx++;
							else historyIdx = history.size();
						} else {
							continue;
						}
		
						clearCurrentLine(prompt.length() + buffer.length());
						buffer = (historyIdx < (int)history.size()) ? history[historyIdx] : "";
						std::cout << prompt << buffer << std::flush;
						cursor = buffer.length();
					} 
					else if (key == 'D') { // LEFT
						if (cursor > 0) {
							cursor--;
							std::cout << "\033[D" << std::flush;
						}
					} 
					else if (key == 'C') { // RIGHT
						if (cursor < (int)buffer.length()) {
							std::cout << "\033[C" << std::flush;
							cursor++;
						}
					}
				}
				continue;
        }
        #endif

        if (c == KEY_ENTER) {
            std::cout << "\n";
            return buffer;
        }

        if (c == KEY_BACKSPACE || c == 127) {
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
