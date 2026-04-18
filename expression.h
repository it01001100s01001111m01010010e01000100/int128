#pragma once

#include "int128.h"
#include <map>
#include <memory>
#include <string>
#include <iostream>


using Arguments = std::map<std::string, Int128>;

struct Expression {
    virtual Int128 eval(const Arguments& values = {}) const = 0;
    virtual Expression* clone() const = 0;
    virtual std::string str() const = 0;

    virtual ~Expression() = default;

    friend std::ostream& operator<<(std::ostream& out, const Expression& e) {
        return out << e.str();
    }
};


struct Const : Expression {
    Int128 value;

    Const(Int128 v) : value(v) {}

    Int128 eval(const Arguments&) const override {
        return value;
    }

    Expression* clone() const override {
        return new Const(value);
    }

    std::string str() const override {
        return value.str();
    }
};


struct Variable : Expression {
    std::string name;

    Variable(std::string n) : name(std::move(n)) {}

    Int128 eval(const Arguments& values) const override {
        return values.at(name);
    }
    

    Expression* clone() const override {
        return new Variable(name);
    }

    std::string str() const override {
        return name;
    }
};


struct BinaryOperation : Expression {
    std::shared_ptr<Expression> a, b;
    char op;

    BinaryOperation(const Expression& x, const Expression& y, char o)
        : a(x.clone()), b(y.clone()), op(o) {}

    BinaryOperation(std::shared_ptr<Expression> x,
                    std::shared_ptr<Expression> y,
                    char o)
        : a(x), b(y), op(o) {}

    std::string str() const override {
        return "(" + a->str() + " " + op + " " + b->str() + ")";
    }
};

struct Negate : Expression {
    std::shared_ptr<Expression> a;

    Negate(const Expression& x) : a(x.clone()) {}
    Negate(std::shared_ptr<Expression> x) : a(x) {}


    Int128 eval(const Arguments& v) const override {
        return -a->eval(v);
    }
    

    Expression* clone() const override {
        return new Negate(a);
    }

    std::string str() const override {
        return "(-" + a->str() + ")";
    }
};


struct Add : BinaryOperation {
    using BinaryOperation::BinaryOperation;

    Int128 eval(const Arguments& v) const override {
        return a->eval(v) + b->eval(v);
    }

    Expression* clone() const override {
        return new Add(a, b, op);
    }
};

struct Subtract : BinaryOperation {
    using BinaryOperation::BinaryOperation;

    Int128 eval(const Arguments& v) const override {
        return a->eval(v) - b->eval(v);
    }

    Expression* clone() const override {
        return new Subtract(a, b, op);
    }
};


struct Multiply : BinaryOperation {
    using BinaryOperation::BinaryOperation;

    Int128 eval(const Arguments& v) const override {
        return a->eval(v) * b->eval(v);
    }

    Expression* clone() const override {
        return new Multiply(a, b, op);
    }
};


struct Divide : BinaryOperation {
    using BinaryOperation::BinaryOperation;

    Int128 eval(const Arguments& v) const override {
        return a->eval(v) / b->eval(v);
    }

    Expression* clone() const override {
        return new Divide(a, b, op);
    }
};


// операторы
Add operator+(const Expression& a, const Expression& b) {
    return Add(a, b, '+');
}

Subtract operator-(const Expression& a, const Expression& b) {
    return Subtract(a, b, '-');
}


Multiply operator*(const Expression& a, const Expression& b) {
    return Multiply(a, b, '*');
}


Divide operator/(const Expression& a, const Expression& b) {
    return Divide(a, b, '/');
}

Negate operator-(const Expression& a) {
    return Negate(a);
}



