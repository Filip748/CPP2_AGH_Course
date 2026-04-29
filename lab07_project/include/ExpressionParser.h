#pragma once

#include <string>
#include <memory>
#include "Expression.h"

class ExpressionParser {
public:
    static std::unique_ptr<Expression> parseRPN(const std::string& input);
};