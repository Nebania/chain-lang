#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "lexer.h"
#include "parser.h"
#include "runtime.h" 
#include "help.h"
#include "repl_core.h"

bool isBlockStart(const std::string& line) {
    size_t start = line.find_first_not_of(" \t");
    if (start == std::string::npos) return false; 
    
    std::string s = line.substr(start);
    size_t end = s.find_last_not_of(" \t");
    if (end != std::string::npos) {
        s = s.substr(0, end + 1);
    }
    if (s.rfind("if", 0) == 0) return true;
    if (s.rfind("while", 0) == 0) return true;
    if (s.rfind("for", 0) == 0) return true;
    if (s.rfind("func", 0) == 0) return true;
    if (s.rfind("app", 0) == 0) return true;
    if (s.rfind("window", 0) == 0) return true;
    if (s.rfind("elif", 0) == 0) return true;
    if (s.rfind("else", 0) == 0) return true;
    if (!s.empty() && s.back() == '{') return true;
    if (!s.empty() && s.back() == '[') return true;
    
    return false;
}

void run(Runtime& runtime, const std::string& source, bool isDebug) {
    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();
        Parser parser(tokens);
        auto program = parser.parse(); 

        if (isDebug) {
            std::cout << "\n--- DEBUG: AST STRUCTURE ---\n";
            program->print();
            std::cout << "----------------------------\n";
        }

        runtime.execute(std::move(program)); 

    } catch (const std::exception& e) {
        std::cerr << "Chain has run into an error: " << e.what() << std::endl;
    }
}

int main(int argc, char** argv) {
    Runtime runtime; 

    if (argc == 2) {
        std::string arg = argv[1];

        if (arg == "--help" || arg == "-h") {
            printHelp();
            return 0;
        }

        if (arg == "--version" || arg == "-v") { 
            printVersion(); 
            return 0;
        }
    }

    // 2. Check the --debug flag
    bool debugMode = false;
    for(int i=1; i<argc; i++) {
        if(std::string(argv[i]) == "--debug") {
            debugMode = true;
            break;
        }
    }

    if (argc < 2 || (argc == 2 && debugMode)) {
        std::cout << "Nebania Chain v0.5 (Analyzer)" << std::endl;
        if (debugMode) std::cout << "[DEBUG MODE ACTIVE]" << std::endl;
        std::cout << "Type 'exit' to quit." << std::endl;
        ReplEditor editor;
        std::string inputBuffer;
        int indentLevel = 0; 

        while (true) {
            std::string prompt = (inputBuffer.empty()) ? "\033[1;32mchain>\033[0m " : "\033[1;33m...  \033[0m ";
            std::string line = editor.readLine(prompt, indentLevel);

            if (line == "exit") break;
            if (!line.empty() && line.back() == '{') {
                indentLevel++;
            }
            if (inputBuffer.empty()) {
                if (isBlockStart(line)) {
                    inputBuffer += line + "\n";
                    editor.addToHistory(line); 
                } else {
                    if (!line.empty()) {
                        editor.addToHistory(line);
                        run(runtime, line, debugMode);
                    }
                }
            } else {
                if (line.empty()) { 
                    editor.addToHistory(inputBuffer); 
                    run(runtime, inputBuffer, debugMode);
                    inputBuffer.clear();
                    indentLevel = 0; 
                } else {
                    if (line.find("}") != std::string::npos) {
                        if (indentLevel > 0) indentLevel--;
                    }
                    inputBuffer += line + "\n";
                }
            }
        }
        return 0;
    }


    std::string filename;
    for(int i=1; i<argc; i++) {
        std::string arg = argv[i];
        if (arg != "--debug") {
            filename = arg;
            break;
        }
    }

    if (filename.empty()) {
        std::cout << "Error: No file specified." << std::endl;
        return 1;
    }

    // .chain is the current script extension. .link is still accepted for
    // backwards compatibility, but is soft-deprecated in favor of .chain.
    auto hasExtension = [](const std::string& name, const std::string& ext) {
        if (name.size() < ext.size()) return false;
        return name.compare(name.size() - ext.size(), ext.size(), ext) == 0;
    };

    if (hasExtension(filename, ".link")) {
        std::cout << "[Tip] This file seems to use the .link extension, which has been deprecated!\nNo worries, Nebania HQ recommends renaming this to a .chain file.\nIt's quick and won't do any harm to your code." << std::endl;
    }

    std::ifstream file(filename);
    if(!file){
        std::cout << "No such file or directory: " << filename << std::endl;
        return 1;
    }

    std::string source((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());

    run(runtime, source, debugMode);

    return 0;        
}

