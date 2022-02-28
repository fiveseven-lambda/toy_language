/**
 * @file token.hpp
 * @brief トークンを定義する
 */
#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <memory>
#include <utility>
#include <string>
#include <optional>

#include "pos.hpp"
#include "expression.hpp"

//! トークンを定義する．
namespace token {
    /**
     * @brief 全てのトークンの基底クラス
     */
    class Token {
    public:
        //! ソースコード中の位置
        pos::Range pos;
        virtual ~Token();
        //! デバッグ用の関数．いずれ消す．
        virtual void debug_print() const = 0;
        virtual std::optional<std::string> identifier();
        virtual std::optional<std::int32_t> positive_integer();
        virtual std::optional<std::int32_t> negative_integer();
        virtual std::optional<expression::UnaryOperator> prefix();
        virtual std::optional<expression::BinaryOperator> infix();
        virtual bool
            is_opening_parenthesis() const,
            is_closing_parenthesis() const,
            is_comma() const;
    };

    /**
     * @brief 識別子 `[a-zA-Z_][a-zA-Z0-9_]*`
     */
    class Identifier : public Token {
        std::string name;
        void debug_print() const override;
        std::optional<std::string> identifier() override;
    public:
        Identifier(std::string);
    };

    /**
     * @brief 整数リテラル `[0-9]+`
     */
    class Integer : public Token {
        std::string value;
        void debug_print() const override;
        std::optional<std::int32_t> positive_integer() override, negative_integer() override;
    public:
        Integer(std::string);
    };

    //! 加算 `+`
    class Plus : public Token {
        void debug_print() const override;
        std::optional<expression::UnaryOperator> prefix() override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! 加算代入 `+=`
    class PlusEqual : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! 減算 `-`
    class Hyphen : public Token {
        void debug_print() const override;
        std::optional<expression::UnaryOperator> prefix() override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! 減算代入 `-=`
    class HyphenEqual : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! 乗算 `*`
    class Asterisk : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! 乗算代入 `*=`
    class AsteriskEqual : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! 除算 `/`
    class Slash : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! 除算代入 `/=`
    class SlashEqual : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! 剰余 `%`
    class Percent : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! 剰余代入 `%=`
    class PercentEqual : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `&`
    class Ampersand : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `&=`
    class AmpersandEqual : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `&&`
    class DoubleAmpersand : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `|`
    class Bar : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `|=`
    class BarEqual : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `||`
    class DoubleBar : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `^`
    class Circumflex : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `^=`
    class CircumflexEqual : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `~`
    class Tilde : public Token {
        void debug_print() const override;
        std::optional<expression::UnaryOperator> prefix() override;
    };
    //! `=`
    class Equal : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `==`
    class DoubleEqual : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `!`
    class Exclamation : public Token {
        void debug_print() const override;
        std::optional<expression::UnaryOperator> prefix() override;
    };
    //! `!=`
    class ExclamationEqual : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `<`
    class Less : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `<=`
    class LessEqual : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `<<`
    class DoubleLess : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `<<=`
    class DoubleLessEqual : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `>`
    class Greater : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `>=`
    class GreaterEqual : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `>>`
    class DoubleGreater : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! `>>=`
    class DoubleGreaterEqual : public Token {
        void debug_print() const override;
        std::optional<expression::BinaryOperator> infix() override;
    };
    //! 開き丸括弧 `(`
    class OpeningParenthesis : public Token {
        void debug_print() const override;
        bool is_opening_parenthesis() const override;
    };
    //! 閉じ丸括弧 `)`
    class ClosingParenthesis : public Token {
        void debug_print() const override;
        bool is_closing_parenthesis() const override;
    };
    //! 開き波括弧 `{`
    class OpeningBrace : public Token {
        void debug_print() const override;
    };
    //! 閉じ波括弧 `}`
    class ClosingBrace : public Token {
        void debug_print() const override;
    };
    //! 開き四角括弧 `[`
    class OpeningBracket : public Token {
        void debug_print() const override;
    };
    //! 閉じ四角括弧 `]`
    class ClosingBracket : public Token {
        void debug_print() const override;
    };
    //! ドット `.`
    class Dot : public Token {
        void debug_print() const override;
    };
    //! コロン `:`
    class Colon : public Token {
        void debug_print() const override;
    };
    //! セミコロン `;`
    class Semicolon : public Token {
        void debug_print() const override;
    };
    //! コンマ `,`
    class Comma : public Token {
        void debug_print() const override;
        bool is_comma() const override;
    };
}

#endif
