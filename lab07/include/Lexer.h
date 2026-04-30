#pragma once

#include <string> 
#include <vector>

enum class TokenType { Number, Variable, Operator };

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
public:
    static std::vector<Token> tokenize(const std::string& input);

private:
    static bool isValidVariableName(const std::string& str);
};