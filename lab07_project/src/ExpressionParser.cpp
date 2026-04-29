#include "../include/ExpressionParser.h"
#include "../include/ASTNodes.h"
#include "../include/Lexer.h"
#include <stack>
#include <stdexcept>
#include <cstdlib>

std::unique_ptr<Expression> ExpressionParser::parseRPN(const std::string& input) {
    std::vector<Token> tokens = Lexer::tokenize(input);

    std::stack<std::unique_ptr<Expression>> stack;

    for(const auto& token : tokens) {
        if(token.type == TokenType::Number) {
            double val = std::strtod(token.value.c_str(), nullptr);
            stack.push(std::make_unique<Constant>(val));
        } else if (token.type == TokenType::Variable) {
            stack.push(std::make_unique<Variable>(token.value));
        } else if(token.type == TokenType::Operator) {
            if(stack.size() < 2) {
                throw std::runtime_error("za mało argumentów na stosie");
            }

            auto right = std::move(stack.top());
            stack.pop();

            auto left = std::move(stack.top());
            stack.pop();

            if (token.value == "+") stack.push(std::make_unique<Add>(std::move(left), std::move(right)));
            else if (token.value == "-") stack.push(std::make_unique<Subtract>(std::move(left), std::move(right)));
            else if (token.value == "*") stack.push(std::make_unique<Multiply>(std::move(left), std::move(right)));
            else if (token.value == "/") stack.push(std::make_unique<Divide>(std::move(left), std::move(right)));
        }
    }

    if(stack.size() != 1) {
        throw std::runtime_error("zly rozmiar stosu na koniec");
    }

    return std::move(stack.top());
}