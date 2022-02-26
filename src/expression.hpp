#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <memory>
#include <utility>
#include <string>
#include <cstddef>

#include "pos.hpp"

/**
 * @file expression.hpp
 * @brief 式
 */

//! 式
namespace expression {
    //! 全ての式の基底クラス
    class Expression {
    public:
        pos::Range pos;
        virtual ~Expression();
        virtual void debug_print(int = 0) const = 0;
    };

    //! 単一の識別子からなる式
    class Identifier : public Expression {
        std::string name;
    public:
        Identifier(std::string);
        void debug_print(int) const override;
    };

    //! 単一の整数リテラルからなる式
    class Integer : public Expression {
        std::int32_t value;
    public:
        Integer(std::int32_t);
        void debug_print(int) const override;
    };

    //! 単項演算子
    enum class UnaryOperator {
        Plus, Minus,
        LogicalNot, BitNot
    };

    class Unary : public Expression {
        UnaryOperator unary_operator;
        std::unique_ptr<Expression> operand;
    public:
        Unary(UnaryOperator, std::unique_ptr<Expression>);
        void debug_print(int) const override;
    };

    //! 2 項演算子
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
        BitAndAssign, BitOrAssign, BitXorAssign,
        RightShiftAssign, LeftShiftAssign
    };

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
