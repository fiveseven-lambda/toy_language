#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <memory>
#include <utility>

#include "pos.hpp"

namespace token {
    class Token {
    public:
        virtual ~Token();
        virtual void debug_print() = 0;
    };
    using TokenWithPos = std::pair<pos::Range, std::unique_ptr<Token>>;

    class Identifier : public Token {
        std::string name;
    public:
        Identifier(std::string &&);
        void debug_print() override;
    };
    class Integer : public Token {
        std::string value;
    public:
        Integer(std::string &&);
        void debug_print() override;
    };
    class Plus : public Token {
        void debug_print() override;
    };
    class PlusEqual : public Token {
        void debug_print() override;
    };
    class Hyphen : public Token {
        void debug_print() override;
    };
    class HyphenEqual : public Token {
        void debug_print() override;
    };
    class Asterisk : public Token {
        void debug_print() override;
    };
    class AsteriskEqual : public Token {
        void debug_print() override;
    };
    class Slash : public Token {
        void debug_print() override;
    };
    class SlashEqual : public Token {
        void debug_print() override;
    };
    class Percent : public Token {
        void debug_print() override;
    };
    class PercentEqual : public Token {
        void debug_print() override;
    };
    class Ampersand : public Token {
        void debug_print() override;
    };
    class AmpersandEqual : public Token {
        void debug_print() override;
    };
    class DoubleAmpersand : public Token {
        void debug_print() override;
    };
    class Bar : public Token {
        void debug_print() override;
    };
    class BarEqual : public Token {
        void debug_print() override;
    };
    class DoubleBar : public Token {
        void debug_print() override;
    };
    class Circumflex : public Token {
        void debug_print() override;
    };
    class CircumflexEqual : public Token {
        void debug_print() override;
    };
    class Tilde : public Token {
        void debug_print() override;
    };
    class Equal : public Token {
        void debug_print() override;
    };
    class DoubleEqual : public Token {
        void debug_print() override;
    };
    class Exclamation : public Token {
        void debug_print() override;
    };
    class ExclamationEqual : public Token {
        void debug_print() override;
    };
    class Less : public Token {
        void debug_print() override;
    };
    class LessEqual : public Token {
        void debug_print() override;
    };
    class DoubleLess : public Token {
        void debug_print() override;
    };
    class DoubleLessEqual : public Token {
        void debug_print() override;
    };
    class Greater : public Token {
        void debug_print() override;
    };
    class GreaterEqual : public Token {
        void debug_print() override;
    };
    class DoubleGreater : public Token {
        void debug_print() override;
    };
    class DoubleGreaterEqual : public Token {
        void debug_print() override;
    };
    class OpeningParenthesis : public Token {
        void debug_print() override;
    };
    class ClosingParenthesis : public Token {
        void debug_print() override;
    };
    class OpeningBrace : public Token {
        void debug_print() override;
    };
    class ClosingBrace : public Token {
        void debug_print() override;
    };
    class OpeningBracket : public Token {
        void debug_print() override;
    };
    class ClosingBracket : public Token {
        void debug_print() override;
    };
    class Dot : public Token {
        void debug_print() override;
    };
    class Colon : public Token {
        void debug_print() override;
    };
    class Semicolon : public Token {
        void debug_print() override;
    };
    class Comma : public Token {
        void debug_print() override;
    };
}

#endif
