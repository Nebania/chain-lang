#pragma once
#include <vector>
#include <string>
#include "token.h"

class Lexer {
public:
    explicit Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    const std::string& src;
    size_t pos;
    int line;
    int column;

    std::vector<int> indentStack;

    char peek() const;
    char advance();
    bool match(char expected);

    void skipWhitespace();
    void skipComment();
    Token makeToken(TokenType type, const std::string& value = "");

    Token identifier();
    Token stringLiteral();
    Token number(); 

    void handleIndentation(std::vector<Token>& tokens);
};

