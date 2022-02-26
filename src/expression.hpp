/**
 * @file expression.hpp
 * @brief 式を定義する
 */
#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <memory>
#include <utility>
#include <string>
#include <cstddef>

#include "pos.hpp"

//! 式を定義する
namespace expression {
    /**
     * @brief 全ての式の基底クラス．
     */
    class Expression {
    public:
        //! ソースコード中の位置．
        pos::Range pos;
        virtual ~Expression();
        //! デバッグ出力用の関数．いずれ消す．
        virtual void debug_print(int = 0) const = 0;
    };

    /**
     * @brief 単一の識別子からなる式．
     */
    class Identifier : public Expression {
        std::string name;
    public:
        Identifier(std::string);
        void debug_print(int) const override;
    };

    /**
     * @brief 単一の整数リテラルからなる式．
     */
    class Integer : public Expression {
        std::int32_t value;
    public:
        Integer(std::int32_t);
        void debug_print(int) const override;
    };

    /**
     * @brief 単項演算子
     */
    enum class UnaryOperator {
        //! 何もしない
        Plus,
        //! 符号の反転
        Minus,
        //! 論理否定
        LogicalNot,
        //! ビット毎の否定
        BitNot
    };

    /**
     * @brief 単項演算
     */
    class UnaryOperation : public Expression {
        UnaryOperator unary_operator;
        std::unique_ptr<Expression> operand;
    public:
        UnaryOperation(UnaryOperator, std::unique_ptr<Expression>);
        void debug_print(int) const override;
    };

    /**
     * @brief 2 項演算子
     */
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

    /**
     * @brief 2項演算
     */
    class BinaryOperation : public Expression {
        BinaryOperator binary_operator;
        std::unique_ptr<Expression> left, right;
    public:
        BinaryOperation(BinaryOperator, std::unique_ptr<Expression>, std::unique_ptr<Expression>);
        void debug_print(int) const override;
    };

    /**
     * @brief 関数呼び出し
     */
    class Invocation : public Expression {
        std::unique_ptr<Expression> function;
        std::vector<std::unique_ptr<Expression>> arguments;
    public:
        Invocation(std::unique_ptr<Expression>, std::vector<std::unique_ptr<Expression>>);
        void debug_print(int) const override;
    };
}

#endif
