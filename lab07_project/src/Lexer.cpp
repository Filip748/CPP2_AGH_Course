#include "../include/Lexer.h"
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <stdexcept>

bool Lexer::isValidVariableName(const std::string& str) {
    if(str.empty()) {
        return false;
    }
    if(!std::isalpha(str[0]) && str[0] != '_') {
        return false;
    }
    for(char c : str) {
        if(!std::isalnum(c) && c != '_') {
            return false;
        }
    }
    return true;
}

std::vector<Token> Lexer::tokenize(const std::string& input) {
    std::vector<Token> tokens;
    std::istringstream stream(input);
    std::string word;

    while(stream >> word) {
        if (word == "+" || word == "-" || word == "*" || word == "/") {
            tokens.push_back({TokenType::Operator, word});
        } else {
            char* endptr = nullptr;
            std::strtod(word.c_str(), &endptr);

            if(*endptr == '\0') {
                tokens.push_back({TokenType::Number, word});
            } else if (isValidVariableName(word)) {
                tokens.push_back({TokenType::Variable, word});
            } else {
                throw std::runtime_error("Nieznany token");
            }
        }
    }
    return tokens;
}