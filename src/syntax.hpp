#ifndef SYNTAX_HPP
#define SYNTAX_HPP

#include <cstddef>
#include <memory>
#include <utility>

#include "pos.hpp"

namespace syntax {
    class Expression {
    public:
        pos::Range pos;
        virtual ~Expression();
        virtual void debug_print(int = 0) const = 0;
    };

    class Identifier : public Expression {
        std::string name;
    public:
        Identifier(std::string);
        void debug_print(int) const override;
    };

    class Integer : public Expression {
        std::int32_t value;
    public:
        Integer(std::int32_t);
        void debug_print(int) const override;
    };

    enum class UnaryOperator {
        Plus, Minus,
        Not, BitNot
    };
    class Unary : public Expression {
        UnaryOperator unary_operator;
        std::unique_ptr<Expression> operand;
    public:
        Unary(UnaryOperator, std::unique_ptr<Expression>);
        void debug_print(int) const override;
    };

    enum class BinaryOperator {
        Mul, Div, Rem,
        Add, Sub,
        LeftShift, RightShift,
        BitAnd, BitOr, BitXor,
        Equal, NotEqual,
        Less, Greater,
        LessEqual, GreaterEqual,
        LogicalAnd, LogicalOr,
        Assign,
        MulAssign, DivAssign, RemAssign, AddAssign, SubAssign,
        BitAndAssign, BitOrAssign, BitXorAssign
    };
    enum Precedence {
        AssignPrecedence,
        LogicalOrPrecedence,
        LogicalAndPrecedence,
        ComparisonPrecedence,
        BitOrPrecedence,
        BitXorPrecedence,
        BitAndPrecedence,
        ShiftPrecedence,
        AddSubPrecedence,
        MulDivRemPrecedence,
        MaxPrecedence
    };
    Precedence precedence(BinaryOperator);
    class Binary : public Expression {
        BinaryOperator binary_operator;
        std::unique_ptr<Expression> left, right;
    public:
        Binary(BinaryOperator, std::unique_ptr<Expression>, std::unique_ptr<Expression>);
        void debug_print(int) const override;
    };

    class Invocation : public Expression {
        std::unique_ptr<Expression> function;
        std::vector<std::unique_ptr<Expression>> arguments;
    public:
        Invocation(std::unique_ptr<Expression>, std::vector<std::unique_ptr<Expression>>);
        void debug_print(int) const override;
    };
}

#endif
