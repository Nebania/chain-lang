#include "lexer.h"
#include "types.h"
#include <cctype>
#include <stdexcept>
#include <unordered_map>

static const std::unordered_map<std::string, TokenType> keywords = {
    {"app", TokenType::APP},            {"for", TokenType::FOR},            {"while", TokenType::WHILE},
    {"if", TokenType::IF},              {"elif", TokenType::ELIF},          {"else", TokenType::ELSE},
    {"window", TokenType::WINDOW},      {"func", TokenType::FUNC},          {"return", TokenType::RETURN},  
    {"expose", TokenType::EXPOSE},      {"connect", TokenType::CONNECT},    {"import", TokenType::IMPORT}, 
   
    {"try", TokenType::TRY}, {"catch", TokenType::CATCH}, {"extern", TokenType::EXTERN},
    {"class", TokenType::CLASS}, {"init", TokenType::INIT}, {"new", TokenType::NEW}, {"this", TokenType::THIS}, 

    {"package", TokenType::PACKAGE},    {"sh", TokenType::SH},
    {"in", TokenType::IN},              {"set", TokenType::SET},
    {"break", TokenType::BREAK},			{"continue", TokenType::CONTINUE}, 
    {"and", TokenType::AND}, 			{"or", TokenType::OR}, 
    {"true", TokenType::TRUE},          {"false", TokenType::FALSE}, 
    {"clear", TokenType::CLEAR},        {"cls", TokenType::CLS},
    {"match", TokenType::MATCH} 
};

Lexer::Lexer(const std::string& source) : src(source), pos(0), line(1), column(1) {
    indentStack.push_back(0);
}

char Lexer::peek() const {
    if (pos >= src.size()) return '\0';
    return src[pos];
}

char Lexer::advance() {
    char c = peek();
    pos++; column++;
    return c;
}

bool Lexer::match(char expected) {
    if (peek() != expected) return false;
    advance();
    return true;
}

Token Lexer::makeToken(TokenType type, const std::string& value) {
    return Token{type, value, line, column};
}

void Lexer::skipWhitespace() {
    while (peek() == ' ' || peek() == '\t' || peek() == '\r') {
        advance();
    }
}

Token Lexer::identifier() {
    int startCol = column;
    std::string value;
    while (std::isalnum(peek()) || peek() == '_') value += advance();
    auto it = keywords.find(value);
    if (it != keywords.end()) return Token{it->second, value, line, startCol};
    return Token{TokenType::IDENTIFIER, value, line, startCol};
}

Token Lexer::stringLiteral() {
    int startCol = column;
    std::string value;
    advance(); 
    
    while (peek() != '"' && peek() != '\0') {
        if (peek() == '\n') {
            value += advance();
            line++;      
            column = 1;  
            continue;
        }
        
        if (peek() == '\\') {
            advance(); 
            if (peek() == '\0') throw ChainError(line, column, "Unterminated string");
            
            char escaped = advance();
            if (escaped == 'n') value += '\n';
            else if (escaped == 't') value += '\t';
            else if (escaped == '"') value += '"';
            else if (escaped == '\\') value += '\\';
            else value += escaped;
        } else {
            value += advance();
        }
    }
    
    if (!match('"')) throw ChainError(line, column, "Unterminated string");
    return Token{TokenType::STRING, value, line, startCol};
}

void Lexer::handleIndentation(std::vector<Token>& tokens) {
    int count = 0;
    while (peek() == ' ') { advance(); count++; }
    if (peek() == '\n' || peek() == '\0') return;
    int currentIndent = indentStack.back();
    if (count > currentIndent) {
        indentStack.push_back(count);
        tokens.push_back(makeToken(TokenType::INDENT));
    } else {
        while (count < currentIndent) {
            indentStack.pop_back();
            currentIndent = indentStack.back();
            tokens.push_back(makeToken(TokenType::DEDENT));
        }
        if (count != currentIndent) throw std::runtime_error("Invalid indentation");
    }
}

void Lexer::skipComment() {
    while (peek() != '\n' && peek() != '\0') advance();
}

Token Lexer::number() {
    int startCol = column;
    std::string value;
    bool isFloat = false;
    while (std::isdigit(peek())) value += advance();
    if (peek() == '.') {
        isFloat = true; value += advance();
        while (std::isdigit(peek())) value += advance();
    }
    return Token{isFloat ? TokenType::TOKEN_FLOAT : TokenType::TOKEN_NUM, value, line, startCol}; 
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (pos < src.size()) {
        char c = peek();
        
        // Exception so #include and #define pass through for extern C++
        if (c == '#') { 
            if (src.substr(pos, 8) == "#include" || src.substr(pos, 7) == "#define") {
                int startCol = column;
                std::string val;
                while(peek() != '\n' && peek() != '\0') val += advance();
                tokens.push_back(Token{TokenType::IDENTIFIER, val, line, startCol});
                continue;
            }
            skipComment(); 
            continue; 
        }
        
        if (c == '\n') { advance(); tokens.push_back(makeToken(TokenType::NEWLINE)); line++; column = 1; handleIndentation(tokens); continue; }
        if (c == ' ' || c == '\t') { skipWhitespace(); continue; }
        if (std::isalpha(c) || c == '_') { 
            Token idToken = identifier();
            tokens.push_back(idToken); 
            
            // --- RAW BLOCK CATCHER FOR EXTERN ---
            if (idToken.type == TokenType::EXTERN) {
                skipWhitespace();
                if (peek() == '"') {
                    tokens.push_back(stringLiteral()); // 1. Tangkap Bahasa (misal: "c")
                    
                    // Lambda helper to skip spaces and newlines without creating a NEWLINE token
                    auto skipSpaceAndNewlines = [&]() {
                        while (peek() == ' ' || peek() == '\t' || peek() == '\n' || peek() == '\r') {
                            if (peek() == '\n') { line++; column = 1; }
                            advance();
                        }
                    };

                    skipSpaceAndNewlines();
                    
                    // 2. Check if there is a second string (Optional flags, e.g. "-O3 -lraylib")
                    if (peek() == '"') {
                        tokens.push_back(stringLiteral());
                        skipSpaceAndNewlines();
                    }
                    
                    // 3. Start capturing raw C++ code
                    if (peek() == '{') {
                        advance(); // Makan '{'
                        tokens.push_back(makeToken(TokenType::LBRACE, "{"));
                        
                        std::string rawCode = "";
                        int braceCount = 1;
                        
                        // Loop penangkap teks murni (Mengabaikan aturan token Chain)
                        while (braceCount > 0 && pos < src.size()) {
                            char nextChar = peek();
                            
                            // Abaikan kurung kurawal di dalam string C++ (misal: print("{"))
                            if (nextChar == '"') {
                                rawCode += advance();
                                while(peek() != '"' && pos < src.size()) {
                                    if (peek() == '\\') rawCode += advance();
                                    rawCode += advance();
                                }
                                if (peek() == '"') rawCode += advance();
                                continue;
                            }
                            
                            if (nextChar == '{') braceCount++;
                            else if (nextChar == '}') {
                                braceCount--;
                                if (braceCount == 0) break; // Berhenti di '}' terakhir
                            }
                            
                            if (nextChar == '\n') { line++; column = 1; }
                            rawCode += advance();
                        }
                        
                        // Masukkan seluruh kode C++ sebagai 1 token string raksasa
                        tokens.push_back(makeToken(TokenType::STRING, rawCode));
                        
                        if (peek() == '}') {
                            advance(); // Makan '}'
                            tokens.push_back(makeToken(TokenType::RBRACE, "}"));
                        }
                    }
                }
            }
            continue; 
        }
        if (std::isdigit(c)) { tokens.push_back(number()); continue; }
        if (c == '"') { tokens.push_back(stringLiteral()); continue; }
        if (c == '\'') { /* Existing single-quote char logic... */ }

        // --- ADDITIONAL LEGAL C++ SYMBOLS TO AVOID UNKNOWN CHAR ---
        if (c == ';') { advance(); tokens.push_back(makeToken(TokenType::SEMICOLON, ";")); continue; }
        if (c == '%') { advance(); tokens.push_back(makeToken(TokenType::IDENTIFIER, "%")); continue; }
        if (c == '~') { advance(); tokens.push_back(makeToken(TokenType::IDENTIFIER, "~")); continue; }
        if (c == '?') { advance(); tokens.push_back(makeToken(TokenType::IDENTIFIER, "?")); continue; }
        if (c == '\\') { advance(); tokens.push_back(makeToken(TokenType::IDENTIFIER, "\\")); continue; }
        if (c == '$') { advance(); tokens.push_back(makeToken(TokenType::IDENTIFIER, "$")); continue; }
        // ---------------------------------------------------------
        
        if (c == '!') {
            advance();
            if (match('=')) tokens.push_back(makeToken(TokenType::BANG_EQ, "!="));
            else tokens.push_back(makeToken(TokenType::BANG, "!"));
            continue;
        }
        
        if (c == '(') { advance(); tokens.push_back(makeToken(TokenType::LPAREN, "(")); continue; }
        if (c == ')') { advance(); tokens.push_back(makeToken(TokenType::RPAREN, ")")); continue; }
        if (c == '[') { advance(); tokens.push_back(makeToken(TokenType::LBRACKET, "[")); continue; }
        if (c == ']') { advance(); tokens.push_back(makeToken(TokenType::RBRACKET, "]")); continue; }
        if (c == '{') { advance(); tokens.push_back(makeToken(TokenType::LBRACE, "{")); continue; }
        if (c == '}') { advance(); tokens.push_back(makeToken(TokenType::RBRACE, "}")); continue; }
        if (c == '.') { advance(); tokens.push_back(makeToken(TokenType::DOT, ".")); continue; }
        if (c == ',') { advance(); tokens.push_back(makeToken(TokenType::COMMA, ",")); continue; }
        if (c == ':') { advance(); tokens.push_back(makeToken(TokenType::COLON, ":")); continue; }

        if (c == '*') { advance(); tokens.push_back(makeToken(TokenType::STAR, "*")); continue; }
        if (c == '/') { advance(); tokens.push_back(makeToken(TokenType::SLASH, "/")); continue; }
        
        if (c == '&') { advance(); tokens.push_back(makeToken(TokenType::BIT_AND, "&")); continue; }
        if (c == '|') { advance(); tokens.push_back(makeToken(TokenType::BIT_OR, "|")); continue; }
        if (c == '^') { advance(); tokens.push_back(makeToken(TokenType::XOR, "^")); continue; }
        
        if (c == '<') {
            advance();
            if (match('<')) tokens.push_back(makeToken(TokenType::LSHIFT, "<<"));
            else if (match('=')) tokens.push_back(makeToken(TokenType::LT, "<=")); // If you implement LE later
            else tokens.push_back(makeToken(TokenType::LT, "<"));
            continue;
        }
        
        if (c == '>') {
            advance();
            if (match('>')) tokens.push_back(makeToken(TokenType::RSHIFT, ">>"));
            else if (match('=')) tokens.push_back(makeToken(TokenType::GT, ">=")); // If you implement GE later
            else tokens.push_back(makeToken(TokenType::GT, ">"));
            continue;
        }
        
        if (c == '=') { 
            advance(); 
            if (match('=')) tokens.push_back(makeToken(TokenType::EQ_EQ, "=="));
            else if (match('>')) tokens.push_back(makeToken(TokenType::FAT_ARROW, "=>"));
            else tokens.push_back(makeToken(TokenType::ASSIGN, "=")); 
            continue; 
        }
        
        if (c == '+') {
            advance();
            if (match('+')) tokens.push_back(makeToken(TokenType::PLUS_PLUS, "++"));
            else if (match('=')) tokens.push_back(makeToken(TokenType::PLUS_EQ, "+=")); 
            else tokens.push_back(makeToken(TokenType::PLUS, "+")); 
            continue;
        }
        
        if (c == '-') {
            advance();
            if (match('>')) tokens.push_back(makeToken(TokenType::ARROW, "->"));
            else if (match('=')) tokens.push_back(makeToken(TokenType::MINUS_EQ, "-=")); 
            else tokens.push_back(makeToken(TokenType::MINUS, "-")); 
            continue;
        }
        
        if (c == '*') { 
            advance(); 
            if (match('=')) tokens.push_back(makeToken(TokenType::STAR_EQ, "*="));
            else tokens.push_back(makeToken(TokenType::STAR, "*")); 
            continue;
        }
        
        if (c == '/') { 
            advance(); 
            if (match('/')) { 
                skipComment();
                continue;
            }
            else if (match('=')) tokens.push_back(makeToken(TokenType::SLASH_EQ, "/=")); 
            else tokens.push_back(makeToken(TokenType::SLASH, "/")); 
            continue;
        }
        
        throw std::runtime_error("Unknown char: " + std::string(1, c));
    }
    tokens.push_back(makeToken(TokenType::EOF_TOKEN));
    return tokens;
}
