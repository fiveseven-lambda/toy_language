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
		virtual bool is_opening_parenthesis(), is_closing_parenthesis(), is_equal(), is_comma(), is_semicolon(), is_colon();
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
	class Equal : public Token {
		bool is_equal() override;	
	};
	class DoubleEqual : public Token {
		std::optional<syntax::BinaryOperator> binary_operator() override;
	};
	class ExclamationEqual : public Token {
		std::optional<syntax::BinaryOperator> binary_operator() override;
	};
	class Exclamation : public Token {
		std::optional<syntax::UnaryOperator> prefix() override;
	};
	class OpeningParenthesis : public Token {
		bool is_opening_parenthesis() override;
	};
	class ClosingParenthesis : public Token {
		bool is_closing_parenthesis() override;
	};
	class Comma : public Token {
		bool is_comma() override;
	};
	class Semicolon : public Token {
		bool is_semicolon() override;	
	};
	class Colon : public Token {
		bool is_colon() override;
	};
}

#endif
