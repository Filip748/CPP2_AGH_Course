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

    std::string toString() const override {
        return std::to_string(value);
    }

    std::unique_ptr<Expression> clone() const override {
        return std::make_unique<Constant>(value);
    }

    std::unique_ptr<Expression> simplify() const override {
        return clone();
    }

    bool isConstant (double& val) const override {
        val = value;
        return true;
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

    std::string toString() const override {
        return name;
    }

    std::unique_ptr<Expression> clone() const override {
        return std::make_unique<Variable>(name);
    }

    std::unique_ptr<Expression> simplify() const override {
        return clone();
    }
};



class Negate : public Expression {
private:
    std::unique_ptr<Expression> child;
public:
    explicit Negate(std::unique_ptr<Expression> c) : child(std::move(c)) {}

    double eval(const Context& context = {}) const override {
        return -child->eval(context);
    }

    const Expression& operator[](size_t index) const override {
        if(index == 0) {
            return *child;
        }
        throw std::out_of_range("Negate ma tylko 1 dziecko");
    }

    std::string toString() const override {
        return "-" + child->toString() + ")";
    }

    std::unique_ptr<Expression> clone() const override {
        return std::make_unique<Negate>(child->clone());
    }

    std::unique_ptr<Expression> simplify() const override {
        auto simplifiedChild = child->simplify();

        double val;
        if(simplifiedChild->isConstant(val)) {
            return std::make_unique<Constant>(-val);
        }
        return std::make_unique<Negate>(std::move(simplifiedChild));
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

    std::string toString() const override {
        return "(" + left->toString() + " + " + right->toString() + ")";
    }

    std::unique_ptr<Expression> clone() const override {
        return std::make_unique<Add>(left->clone(), right->clone());
    }

    std::unique_ptr<Expression> simplify() const override {
        auto l = left->simplify();
        auto r = right->simplify();

        double lval, rval;
        bool lconst = l->isConstant(lval);
        bool rconst = r->isConstant(rval);

        if(lconst && rconst) {
            return std::make_unique<Constant>(lval + rval);
        }

        if (lconst && lval == 0.0) return r; // 0 + x -> x
        if (rconst && rval == 0.0) return l; // x + 0 -> x

        return std::make_unique<Add>(std::move(l), std::move(r));
    }
};

class Subtract : public BinaryOperator {
public:
    using BinaryOperator::BinaryOperator;
    double eval(const Context& context = {}) const override {
        return left->eval(context) - right->eval(context);
    }

    std::string toString() const override {
        return "(" + left->toString() + " - " + right->toString() + ")";
    }

    std::unique_ptr<Expression> clone() const override {
        return std::make_unique<Subtract>(left->clone(), right->clone());
    }

    std::unique_ptr<Expression> simplify() const override {
        auto l = left->simplify();
        auto r = right->simplify();

        double rval, lval;
        bool lconst = l->isConstant(lval);
        bool rconst = r->isConstant(rval);

        if(lconst && rconst) {
            return std::make_unique<Constant>(lval - rval);
        }

        if(rconst && rval == 0.0) {
            return l;
        }

        if(lconst && lval == 0.0) {
            return std::make_unique<Negate>(std::move(r));
        }

        return std::make_unique<Subtract>(std::move(l), std::move(r));
    }
};

class Multiply : public BinaryOperator {
public:
    using BinaryOperator::BinaryOperator;
    double eval(const Context& context = {}) const override {
        return left->eval(context) * right->eval(context);
    }

    std::string toString() const override {
        return "(" + left->toString() + " * " + right->toString() + ")";
    }

    std::unique_ptr<Expression> clone() const override {
        return std::make_unique<Multiply>(left->clone(), right->clone());
    }

    std::unique_ptr<Expression> simplify() const override {
        auto l = left->simplify();
        auto r = right->simplify();

        double lval, rval;
        bool lconst = l->isConstant(lval);
        bool rconst = r->isConstant(rval);

        if(lconst && rconst) {
            return std::make_unique<Constant>(lval * rval);
        }

        if((lconst && lval == 0.0) || (rconst && rval == 0.0)) {
            return std::make_unique<Constant>(0.0);
        }

        if(lconst && lval == 1.0) return r;
        if(rconst && rval == 1.0) return l;

        return std::make_unique<Multiply>(std::move(l), std::move(r));
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

    std::string toString() const override {
        return "(" + left->toString() + " / " + right->toString() + ")";
    }

    std::unique_ptr<Expression> clone() const override {
        return std::make_unique<Divide>(left->clone(), right->clone());
    }

    std::unique_ptr<Expression> simplify() const override {
        auto l = left->simplify();
        auto r = right->simplify();

        double lval, rval;
        bool lconst = l->isConstant(lval);
        bool rconst = r->isConstant(rval);

        if (lconst && rconst) {
        if (rval == 0.0) throw std::runtime_error("Dzielenie przez zero");
        return std::make_unique<Constant>(lval / rval);
    }

        if (rconst && rval == 1.0) {
            return l;
        }

        return std::make_unique<Divide>(std::move(l), std::move(r));
    }
};