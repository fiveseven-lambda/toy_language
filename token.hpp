#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <cstdint>
#include <string>
#include <optional>
#include "syntax.hpp"

namespace token {
	class Token {
	public:
		virtual ~Token();
		virtual std::optional<syntax::BinaryOperator> binary_operator();
		virtual void print() = 0;
	};
	class Identifier : public Token {
		std::string name;
	public:
		Identifier(std::string);
		void print() override;
	};
	class Integer : public Token {
		std::string value;
	public:
		Integer(std::string);
		void print() override;
	};
	class Plus : public Token {
		std::optional<syntax::BinaryOperator> binary_operator() override;
		void print() override;
	};
	class Hyphen : public Token {
		std::optional<syntax::BinaryOperator> binary_operator() override;
		void print() override;
	};
	class Asterisk : public Token {
		std::optional<syntax::BinaryOperator> binary_operator() override;
		void print() override;
	};
	class Slash : public Token {
		std::optional<syntax::BinaryOperator> binary_operator() override;
		void print() override;
	};
}

#endif
