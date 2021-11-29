#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <memory>
#include <utility>
#include <string>
#include <optional>

#include "pos.hpp"
#include "syntax/expression.hpp"
#include "syntax/type.hpp"

namespace token {
    class Token {
    public:
        pos::Range pos;
        virtual ~Token();
        virtual void debug_print() const = 0;
        virtual std::optional<std::string> identifier();
        virtual std::unique_ptr<syntax::type::Type> type_name();
        virtual std::optional<std::int32_t> positive_integer(), negative_integer();
        virtual std::optional<syntax::expression::UnaryOperator> prefix();
        virtual std::optional<syntax::expression::BinaryOperator> binary_operator();
        virtual bool is_opening_parenthesis(), is_closing_parenthesis(), is_comma(), is_semicolon(), is_colon(), is_equal();
    };

    class Identifier : public Token {
        std::string name;
    public:
        Identifier(std::string);
        void debug_print() const override;
        std::optional<std::string> identifier() override;
        std::unique_ptr<syntax::type::Type> type_name() override;
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
        std::optional<syntax::expression::UnaryOperator> prefix() override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class PlusEqual : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class Hyphen : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::UnaryOperator> prefix() override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class HyphenEqual : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class Asterisk : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class AsteriskEqual : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class Slash : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class SlashEqual : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class Percent : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class PercentEqual : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class Ampersand : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class AmpersandEqual : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class DoubleAmpersand : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class Bar : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class BarEqual : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class DoubleBar : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class Circumflex : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class CircumflexEqual : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class Tilde : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::UnaryOperator> prefix() override;
    };
    class Equal : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
        bool is_equal() override;
    };
    class DoubleEqual : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class Exclamation : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::UnaryOperator> prefix() override;
    };
    class ExclamationEqual : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class Less : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class LessEqual : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class DoubleLess : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class DoubleLessEqual : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class Greater : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class GreaterEqual : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class DoubleGreater : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class DoubleGreaterEqual : public Token {
        void debug_print() const override;
        std::optional<syntax::expression::BinaryOperator> binary_operator() override;
    };
    class OpeningParenthesis : public Token {
        void debug_print() const override;
        bool is_opening_parenthesis() override;
    };
    class ClosingParenthesis : public Token {
        void debug_print() const override;
        bool is_closing_parenthesis() override;
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
        bool is_colon() override;
    };
    class Semicolon : public Token {
        void debug_print() const override;
        bool is_semicolon() override;
    };
    class Comma : public Token {
        void debug_print() const override;
        bool is_comma() override;
    };
}

#endif
