/**
 * @file expression.hpp
 * @brief 式を定義する
 */
#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <memory>
#include <optional>

#include "pos.hpp"

/**
 * @brief 式を定義する
 *
 * @code
 * <Invocation> ::= <Identifier>
 *                | <Integer>
 *                | `(` <Expression> `)`
 *                | <Invocation> `(` <List> `)`
 * <Factor> ::= <Invocation>
 *            | <UnaryOperator> <Factor>
 * <Term> ::= <Factor>
 *          | <Term> `*` <Factor>
 *          | <Term> `/` <Factor>
 *          | <Term> `%` <Factor>
 * <AddSub> ::= <Term>
 *            | <AddSub> `+` <Term>
 *            | <AddSub> `-` <Term>
 * <Shift> ::= <AddSub>
 *           | <Shift> `<<` <AddSub>
 *           | <Shift> `>>` <AddSub>
 * <BitAnd> ::= <Shift>
 *            | <BitAnd> `&` <Shift>
 * <BitXor> ::= <BitAnd>
 *            | <BitXor> `^` <BitAnd>
 * <BitOr> ::= <BitXor>
 *           | <BitOr> `|` <BitXor>
 * <Comparison> ::= <BitOr>
 *                | <Comparison> `==` <BitOr>
 *                | <Comparison> `!=` <BitOr>
 *                | <Comparison> `<` <BitOr>
 *                | <Comparison> `<=` <BitOr>
 *                | <Comparison> `>` <BitOr>
 *                | <Comparison> `>=` <BitOr>
 * <LogicalAnd> ::= <Comparison>
 *                | <LogicalAnd> `&&` <Comparison>
 * <LogicalOr> ::= <LogicalAnd>
 *               | <LogicalOr> `||` <LogicalAnd>
 * <Expression> ::= <LogicalOr>
 *                | <LogicalOr> `=` <Expression>
 *                | <LogicalOr> `+=` <Expression>
 *                | <LogicalOr> `-=` <Expression>
 *                | <LogicalOr> `*=` <Expression>
 *                | <LogicalOr> `/=` <Expression>
 *                | <LogicalOr> `%=` <Expression>
 *                | <LogicalOr> `<<=` <Expression>
 *                | <LogicalOr> `>>=` <Expression>
 *                | <LogicalOr> `&=` <Expression>
 *                | <LogicalOr> `|=` <Expression>
 *                | <LogicalOr> `^=` <Expression>
 * <List> ::= ( <Expression> `,` )* <Expression>?
 * @endcode
 */
namespace expression {
    /**
     * @brief 全ての式の基底クラス．
     */
    class Expression {
    public:
        //! ソースコード中の位置．
        pos::Range pos;
        virtual ~Expression();
        virtual std::optional<std::string> identifier();
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
        std::optional<std::string> identifier() override;
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
        //! 加算
        Add,
        //! 減算
        Sub,
        //! 乗算
        Mul,
        //! 除算
        Div,
        //! 剰余演算
        Rem,
        //! 左シフト
        LeftShift,
        //! 右シフト
        RightShift,
        //! 等しい
        Equal,
        //! 等しくない
        NotEqual,
        //! 左が大きい
        Less,
        //! 右が大きい
        Greater,
        //! 等しいか左が大きい
        LessEqual,
        //! 等しいか右が大きい
        GreaterEqual,
        //! 論理積
        LogicalAnd,
        //! 論理和
        LogicalOr,
        //! ビット毎論理積
        BitAnd,
        //! ビット毎論理和
        BitOr,
        //! ビット毎排他的論理和
        BitXor,
        //! 代入
        Assign,
        //! 加算代入
        AddAssign,
        //! 減算代入
        SubAssign,
        //! 乗算代入
        MulAssign,
        //! 除算代入
        DivAssign,
        //! 剰余代入
        RemAssign,
        //! ビット毎論理積代入
        BitAndAssign,
        //! ビット毎論理和代入
        BitOrAssign,
        //! ビット毎排他的論理和代入
        BitXorAssign,
        //! 右シフト代入
        RightShiftAssign,
        //! 左シフト代入
        LeftShiftAssign
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
     * @brief 括弧でくくられた式
     */
    class Group : public Expression {
        std::unique_ptr<Expression> expression;
    public:
	Group(std::unique_ptr<Expression>);
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
