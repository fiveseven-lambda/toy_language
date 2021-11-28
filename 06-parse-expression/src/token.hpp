#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <memory>
#include <utility>
#include <string>
#include <optional>

#include "pos.hpp"
#include "syntax.hpp"

namespace token {
    class Token {
    public:
        pos::Range pos;
        virtual ~Token();
        virtual void debug_print() const = 0;
        virtual std::optional<std::string> identifier();
        virtual std::optional<std::int32_t> positive_integer(), negative_integer();
        virtual std::optional<syntax::UnaryOperator> prefix();
    };

    class Identifier : public Token {
        std::string name;
    public:
        Identifier(std::string);
        void debug_print() const override;
        std::optional<std::string> identifier() override;
    };

    class Integer : public Token {
        std::string value;
    public:
        Integer(std::string);
        void debug_print() const override;
        std::optional<std::int32_t> positive_integer() override, negative_integer() override;
    };

    class Plus : public Token {
        void debug_print() const override;
        std::optional<syntax::UnaryOperator> prefix() override;
    };
    class PlusEqual : public Token {
        void debug_print() const override;
    };
    class Hyphen : public Token {
        void debug_print() const override;
        std::optional<syntax::UnaryOperator> prefix() override;
    };
    class HyphenEqual : public Token {
        void debug_print() const override;
    };
    class Asterisk : public Token {
        void debug_print() const override;
    };
    class AsteriskEqual : public Token {
        void debug_print() const override;
    };
    class Slash : public Token {
        void debug_print() const override;
    };
    class SlashEqual : public Token {
        void debug_print() const override;
    };
    class Percent : public Token {
        void debug_print() const override;
    };
    class PercentEqual : public Token {
        void debug_print() const override;
    };
    class Ampersand : public Token {
        void debug_print() const override;
    };
    class AmpersandEqual : public Token {
        void debug_print() const override;
    };
    class DoubleAmpersand : public Token {
        void debug_print() const override;
    };
    class Bar : public Token {
        void debug_print() const override;
    };
    class BarEqual : public Token {
        void debug_print() const override;
    };
    class DoubleBar : public Token {
        void debug_print() const override;
    };
    class Circumflex : public Token {
        void debug_print() const override;
    };
    class CircumflexEqual : public Token {
        void debug_print() const override;
    };
    class Tilde : public Token {
        void debug_print() const override;
        std::optional<syntax::UnaryOperator> prefix() override;
    };
    class Equal : public Token {
        void debug_print() const override;
    };
    class DoubleEqual : public Token {
        void debug_print() const override;
    };
    class Exclamation : public Token {
        void debug_print() const override;
        std::optional<syntax::UnaryOperator> prefix() override;
    };
    class ExclamationEqual : public Token {
        void debug_print() const override;
    };
    class Less : public Token {
        void debug_print() const override;
    };
    class LessEqual : public Token {
        void debug_print() const override;
    };
    class DoubleLess : public Token {
        void debug_print() const override;
    };
    class DoubleLessEqual : public Token {
        void debug_print() const override;
    };
    class Greater : public Token {
        void debug_print() const override;
    };
    class GreaterEqual : public Token {
        void debug_print() const override;
    };
    class DoubleGreater : public Token {
        void debug_print() const override;
    };
    class DoubleGreaterEqual : public Token {
        void debug_print() const override;
    };
    class OpeningParenthesis : public Token {
        void debug_print() const override;
    };
    class ClosingParenthesis : public Token {
        void debug_print() const override;
    };
    class OpeningBrace : public Token {
        void debug_print() const override;
    };
    class ClosingBrace : public Token {
        void debug_print() const override;
    };
    class OpeningBracket : public Token {
        void debug_print() const override;
    };
    class ClosingBracket : public Token {
        void debug_print() const override;
    };
    class Dot : public Token {
        void debug_print() const override;
    };
    class Colon : public Token {
        void debug_print() const override;
    };
    class Semicolon : public Token {
        void debug_print() const override;
    };
    class Comma : public Token {
        void debug_print() const override;
    };
}

#endif
