#ifndef SYNTAX_HPP
#define SYNTAX_HPP

#include <cstdlib>
#include <string>
#include <memory>

namespace syntax {
    class Expression {
    public:
        virtual ~Expression();
        virtual void print(int = 0) = 0;
    };
    class Identifier : public Expression {
        std::string name;
    public:
        Identifier(std::string);
        void print(int) override;
    };
    class Integer : public Expression {
        std::int32_t value;
    public:
        Integer(std::int32_t);
        void print(int) override;
    };
    enum class UnaryOperator {
        Plus, Minus
    };
    class Unary : public Expression {
        std::unique_ptr<Expression> operand;
        UnaryOperator unary_operator;
    public:
        Unary(std::unique_ptr<Expression>, UnaryOperator);
        void print(int) override;
    };
    enum class BinaryOperator {
        Add, Sub, Mul, Div
    };
    class Binary : public Expression {
        std::unique_ptr<Expression> left, right;
        BinaryOperator binary_operator;
    public:
        Binary(std::unique_ptr<Expression>, std::unique_ptr<Expression>, BinaryOperator);
        void print(int) override;
    };
}

#endif
