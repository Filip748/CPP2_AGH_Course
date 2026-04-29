#pragma once

#include "Expression.h"
#include <string>
#include <memory>
#include <stdexcept>

class Constant : public Expression {
public:
    explicit Constant(double val) : value(val) {}

    double eval(const Context& /*context*/ = {}) const override { return value; }

    const Expression& operator[](size_t /*index*/) const override {
        throw std::out_of_range("Constatnt nie ma dzieci");
    }

private:
    double value;
};



class Variable : public Expression {
private:
    std::string name;

public:
    explicit Variable(const std::string& n) : name(n) {}

    double eval(const Context& context = {}) const override {
        auto it = context.find(name);
        if(it != context.end()) {
            return it->second;
        }
        throw std::runtime_error(name);
    }

    const Expression& operator[](size_t /*index*/) const override {
        throw std::out_of_range("Variable nie ma dzieci");
    }
};

class BinaryOperator : public Expression {
protected:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;

public:
    BinaryOperator(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r)
        : left(std::move(l)), right(std::move(r)) {}

    const Expression& operator[](size_t index) const override {
        if(index == 0) return *left;
        if(index == 1) return *right;
        throw std::out_of_range("BinaryOperator ma tylko 2 dzieci");
    }
};

class Add : public BinaryOperator {
public:
    using BinaryOperator::BinaryOperator;
    double eval(const Context& context = {}) const override {
        return left->eval(context) + right->eval(context);
    }
};

class Subtract : public BinaryOperator {
public:
    using BinaryOperator::BinaryOperator;
    double eval(const Context& context = {}) const override {
        return left->eval(context) - right->eval(context);
    }
};

class Multiply : public BinaryOperator {
public:
    using BinaryOperator::BinaryOperator;
    double eval(const Context& context = {}) const override {
        return left->eval(context) * right->eval(context);
    }
};

class Divide : public BinaryOperator {
public:
    using BinaryOperator::BinaryOperator;
    double eval(const Context& context = {}) const override {
        double rValue = right->eval(context);
        if (rValue == 0.0) throw std::runtime_error("Dzielenie przez zero");
        return left->eval(context) / rValue;
    }
};