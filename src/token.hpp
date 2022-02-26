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
        //! 識別子ならば中身を `std::string` で返し，そうでなければ `std::nullopt` を返す．
        virtual std::optional<std::string> identifier();
        //! 整数リテラルなら正の数として `std::int32_t` で読み取って返し，そうでなければ `std::nullopt` を返す．
        virtual std::optional<std::int32_t> positive_integer();
        //! 整数リテラルなら負の数として `std::int32_t` で読み取って返し，そうでなければ `std::nullopt` を返す．
        virtual std::optional<std::int32_t> negative_integer();
        //! 前置単項演算子なら対応する `expression::UnaryOperator` を返し，そうでなければ `std::nullopt` を返す．
        virtual std::optional<expression::UnaryOperator> prefix();
        /**
         * @retval true 開き括弧 `(`
         * @retval false それ以外
         */
        virtual bool is_opening_parenthesis() const;
        /**
         * @retval true 閉じ括弧 `)`
         * @retval false それ以外
         */
        virtual bool is_closing_parenthesis() const;
    };

    /**
     * @brief 識別子
     */
    class Identifier : public Token {
        std::string name;
        void debug_print() const override;
        std::optional<std::string> identifier() override;
    public:
        Identifier(std::string);
    };

    /**
     * @brief 整数リテラル．
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
    };
    //! 加算代入 `+=`
    class PlusEqual : public Token {
        void debug_print() const override;
    };
    //! 減算 `-`
    class Hyphen : public Token {
        void debug_print() const override;
        std::optional<expression::UnaryOperator> prefix() override;
    };
    //! 減算代入 `-=`
    class HyphenEqual : public Token {
        void debug_print() const override;
    };
    //! 乗算 `*`
    class Asterisk : public Token {
        void debug_print() const override;
    };
    //! 乗算代入 `*=`
    class AsteriskEqual : public Token {
        void debug_print() const override;
    };
    //! 除算 `/`
    class Slash : public Token {
        void debug_print() const override;
    };
    //! 除算代入 `/=`
    class SlashEqual : public Token {
        void debug_print() const override;
    };
    //! 剰余 `%`
    class Percent : public Token {
        void debug_print() const override;
    };
    //! 剰余代入 `%=`
    class PercentEqual : public Token {
        void debug_print() const override;
    };
    //! `&`
    class Ampersand : public Token {
        void debug_print() const override;
    };
    //! `&=`
    class AmpersandEqual : public Token {
        void debug_print() const override;
    };
    //! `&&`
    class DoubleAmpersand : public Token {
        void debug_print() const override;
    };
    //! `|`
    class Bar : public Token {
        void debug_print() const override;
    };
    //! `|=`
    class BarEqual : public Token {
        void debug_print() const override;
    };
    //! `||`
    class DoubleBar : public Token {
        void debug_print() const override;
    };
    //! `^`
    class Circumflex : public Token {
        void debug_print() const override;
    };
    //! `^=`
    class CircumflexEqual : public Token {
        void debug_print() const override;
    };
    //! `~`
    class Tilde : public Token {
        void debug_print() const override;
        std::optional<expression::UnaryOperator> prefix() override;
    };
    //! `=`
    class Equal : public Token {
        void debug_print() const override;
    };
    //! `==`
    class DoubleEqual : public Token {
        void debug_print() const override;
    };
    //! `!`
    class Exclamation : public Token {
        void debug_print() const override;
        std::optional<expression::UnaryOperator> prefix() override;
    };
    //! `!=`
    class ExclamationEqual : public Token {
        void debug_print() const override;
    };
    //! `<`
    class Less : public Token {
        void debug_print() const override;
    };
    //! `<=`
    class LessEqual : public Token {
        void debug_print() const override;
    };
    //! `<<`
    class DoubleLess : public Token {
        void debug_print() const override;
    };
    //! `<<=`
    class DoubleLessEqual : public Token {
        void debug_print() const override;
    };
    //! `>`
    class Greater : public Token {
        void debug_print() const override;
    };
    //! `>=`
    class GreaterEqual : public Token {
        void debug_print() const override;
    };
    //! `>>`
    class DoubleGreater : public Token {
        void debug_print() const override;
    };
    //! `>>=`
    class DoubleGreaterEqual : public Token {
        void debug_print() const override;
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
    };
}

#endif
