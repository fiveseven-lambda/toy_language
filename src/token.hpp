/**
 * @file token.hpp
 * @brief トークンを定義する
 */
#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "expression.hpp"
#include "type.hpp"

//! トークンを定義する．
namespace token {
    //! キーワード
    enum class Keyword {
        //! `if` 条件分岐
        If,
        //! `else` 条件分岐
        Else,
        //! `while` ループ
        While
    };

    /**
     * @brief 全てのトークンの基底クラス
     */
    class Token {
    public:
        //! ソースコード中の位置
        pos::Range pos;
        virtual ~Token();
        virtual std::optional<std::string> identifier();
        virtual std::optional<Keyword> keyword();
        virtual std::unique_ptr<type::Type> primitive_type();
        virtual std::optional<std::int32_t> positive_integer();
        virtual std::optional<std::int32_t> negative_integer();
        virtual std::optional<expression::UnaryOperator> prefix();
        virtual std::optional<expression::BinaryOperator> infix();
        virtual bool
            is_equal() const,
            is_opening_parenthesis() const,
            is_closing_parenthesis() const,
            is_opening_brace() const,
            is_closing_brace() const,
            is_opening_bracket() const,
            is_closing_bracket() const,
            is_dot() const,
            is_colon() const,
            is_semicolon() const,
            is_comma() const;
    };

    /**
     * @brief 識別子 `[a-zA-Z_][a-zA-Z0-9_]*`
     */
    class Identifier : public Token {
        std::string name;
        std::optional<std::string> identifier() override;
        std::optional<Keyword> keyword() override;
        std::unique_ptr<type::Type> primitive_type() override;
    public:
        Identifier(std::string);
    };

    /**
     * @brief 整数リテラル `[0-9]+`
     */
    class Integer : public Token {
        std::string value;
        std::optional<std::int32_t> positive_integer() override, negative_integer() override;
    public:
        Integer(std::string);
    };

    //! 加算 `+`
    class Plus : public Token {
        std::optional<expression::UnaryOperator> prefix() override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! 加算代入 `+=`
    class PlusEqual : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! 減算 `-`
    class Hyphen : public Token {
        std::optional<expression::UnaryOperator> prefix() override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! 減算代入 `-=`
    class HyphenEqual : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! 乗算 `*`
    class Asterisk : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! 乗算代入 `*=`
    class AsteriskEqual : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! 除算 `/`
    class Slash : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! 除算代入 `/=`
    class SlashEqual : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! 剰余 `%`
    class Percent : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! 剰余代入 `%=`
    class PercentEqual : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `&`
    class Ampersand : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `&=`
    class AmpersandEqual : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `&&`
    class DoubleAmpersand : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `|`
    class Bar : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `|=`
    class BarEqual : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `||`
    class DoubleBar : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `^`
    class Circumflex : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `^=`
    class CircumflexEqual : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `~`
    class Tilde : public Token {
        std::optional<expression::UnaryOperator> prefix() override;
    };
    //! `=`
    class Equal : public Token {
        std::optional<expression::BinaryOperator> infix() override;
        bool is_equal() const override;
    };
    //! `==`
    class DoubleEqual : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `!`
    class Exclamation : public Token {
        std::optional<expression::UnaryOperator> prefix() override;
    };
    //! `!=`
    class ExclamationEqual : public Token {
        
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `<`
    class Less : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `<=`
    class LessEqual : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `<<`
    class DoubleLess : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `<<=`
    class DoubleLessEqual : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `>`
    class Greater : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `>=`
    class GreaterEqual : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `>>`
    class DoubleGreater : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `>>=`
    class DoubleGreaterEqual : public Token {
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! 開き丸括弧 `(`
    class OpeningParenthesis : public Token {
        bool is_opening_parenthesis() const override;
    };
    //! 閉じ丸括弧 `)`
    class ClosingParenthesis : public Token {
        bool is_closing_parenthesis() const override;
    };
    //! 開き波括弧 `{`
    class OpeningBrace : public Token {
        bool is_opening_brace() const override;
    };
    //! 閉じ波括弧 `}`
    class ClosingBrace : public Token {
        bool is_closing_brace() const override;
    };
    //! 開き四角括弧 `[`
    class OpeningBracket : public Token {
        bool is_opening_bracket() const override;
    };
    //! 閉じ四角括弧 `]`
    class ClosingBracket : public Token {
        bool is_closing_bracket() const override;
    };
    //! ドット `.`
    class Dot : public Token {
        bool is_dot() const override;
    };
    //! コロン `:`
    class Colon : public Token {
        bool is_colon() const override;
    };
    //! セミコロン `;`
    class Semicolon : public Token {
        bool is_semicolon() const override;
    };
    //! コンマ `,`
    class Comma : public Token {
        bool is_comma() const override;
    };
}

#endif
