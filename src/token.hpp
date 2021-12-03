#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <memory>
#include <utility>
#include <string>
#include <optional>

#include "pos.hpp"
#include "expression.hpp"
#include "type.hpp"

namespace token {
    class Token {
    public:
        pos::Range pos;
        virtual ~Token();
        virtual std::optional<std::string> identifier();
        virtual std::shared_ptr<type::Type> type_name();
        virtual std::optional<std::int32_t> positive_integer(), negative_integer();
        virtual std::optional<expression::UnaryOperator> prefix();
        virtual std::optional<expression::BinaryOperator> binary_operator();
        virtual bool
            is_equal(),
            is_opening_parenthesis(),
            is_closing_parenthesis(),
            is_opening_brace(),
            is_closing_brace(),
            is_opening_bracket(),
            is_closing_bracket(),
            is_dot(),
            is_comma(),
            is_semicolon(),
            is_colon();
    };

    class Identifier : public Token {
        std::string name;
    public:
        Identifier(std::string);
        std::optional<std::string> identifier() override;
        std::shared_ptr<type::Type> type_name() override;
    };

    class Integer : public Token {
        std::string value;
    public:
        Integer(std::string);
        std::optional<std::int32_t> positive_integer() override, negative_integer() override;
    };

    class Plus : public Token {
        std::optional<expression::UnaryOperator> prefix() override;
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class PlusEqual : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class Hyphen : public Token {
        std::optional<expression::UnaryOperator> prefix() override;
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class HyphenEqual : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class Asterisk : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class AsteriskEqual : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class Slash : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class SlashEqual : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class Percent : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class PercentEqual : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class Ampersand : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class AmpersandEqual : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class DoubleAmpersand : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class Bar : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class BarEqual : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class DoubleBar : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class Circumflex : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class CircumflexEqual : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class Tilde : public Token {
        std::optional<expression::UnaryOperator> prefix() override;
    };
    class Equal : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
        bool is_equal() override;
    };
    class DoubleEqual : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class Exclamation : public Token {
        std::optional<expression::UnaryOperator> prefix() override;
    };
    class ExclamationEqual : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class Less : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class LessEqual : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class DoubleLess : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class DoubleLessEqual : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class Greater : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class GreaterEqual : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class DoubleGreater : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class DoubleGreaterEqual : public Token {
        std::optional<expression::BinaryOperator> binary_operator() override;
    };
    class OpeningParenthesis : public Token {
        bool is_opening_parenthesis() override;
    };
    class ClosingParenthesis : public Token {
        bool is_closing_parenthesis() override;
    };
    class OpeningBrace : public Token {
        bool is_opening_brace() override;
    };
    class ClosingBrace : public Token {
        bool is_closing_brace() override;
    };
    class OpeningBracket : public Token {
        bool is_opening_bracket() override;
    };
    class ClosingBracket : public Token {
        bool is_closing_bracket() override;
    };
    class Dot : public Token {
        bool is_dot() override;
    };
    class Colon : public Token {
        bool is_colon() override;
    };
    class Semicolon : public Token {
        bool is_semicolon() override;
    };
    class Comma : public Token {
        bool is_comma() override;
    };
}

#endif
