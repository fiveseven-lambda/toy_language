#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <cstdint>
#include <string>
#include <optional>
#include "syntax.hpp"

namespace token {
	class Token {
		Token(const Token &) = delete;
        Token &operator=(const Token &) = delete;
	protected:
		Token();
	public:
		virtual ~Token();
		virtual std::optional<std::string> identifier();
		virtual std::optional<std::int32_t> positive_integer(), negative_integer();
		virtual std::optional<syntax::UnaryOperator> prefix();
		virtual std::optional<syntax::BinaryOperator> binary_operator();
		virtual bool is_opening_parenthesis(), is_closing_parenthesis();
	};
	class Identifier : public Token {
		std::string name;
	public:
		Identifier(std::string);
		std::optional<std::string> identifier() override;
	};
	class Integer : public Token {
		std::string value;
	public:
		std::optional<std::int32_t> positive_integer() override, negative_integer() override;
		Integer(std::string);
	};
	class Plus : public Token {
		std::optional<syntax::UnaryOperator> prefix() override;
		std::optional<syntax::BinaryOperator> binary_operator() override;
	};
	class Hyphen : public Token {
		std::optional<syntax::UnaryOperator> prefix() override;
		std::optional<syntax::BinaryOperator> binary_operator() override;
	};
	class Asterisk : public Token {
		std::optional<syntax::BinaryOperator> binary_operator() override;
	};
	class Slash : public Token {
		std::optional<syntax::BinaryOperator> binary_operator() override;
	};
	class OpeningParenthesis : public Token {
		bool is_opening_parenthesis() override;
	};
	class ClosingParenthesis : public Token {
		bool is_closing_parenthesis() override;
	};
}

#endif
