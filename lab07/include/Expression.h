#pragma once

#include <map>
#include <string>
#include <stdexcept>

class Expression { 
public:
    using Context = std::map<std::string, double>;

    virtual ~Expression() = default;

    virtual double eval(const Context& context = {}) const = 0;

    virtual const Expression& operator[](size_t index) const = 0;


    virtual std::string toString() const = 0;
    virtual std::unique_ptr<Expression> clone() const = 0;
    virtual std::unique_ptr<Expression> simplify() const = 0;
    virtual bool isConstant(double& ) const { return false; }
};