
#include "token.hpp"

#include <iostream>

namespace token {
    Token::~Token() = default;
    Identifier::Identifier(std::string name):
        name(std::move(name)) {}
    Integer::Integer(std::string value):
        value(std::move(value)) {}

    std::optional<syntax::BinaryOperator> Token::binary_operator(){ return std::nullopt; }
    std::optional<syntax::BinaryOperator> Plus::binary_operator(){ return syntax::BinaryOperator::Add; }
    std::optional<syntax::BinaryOperator> Hyphen::binary_operator(){ return syntax::BinaryOperator::Sub; }
    std::optional<syntax::BinaryOperator> Asterisk::binary_operator(){ return syntax::BinaryOperator::Mul; }
    std::optional<syntax::BinaryOperator> Slash::binary_operator(){ return syntax::BinaryOperator::Div; }

    void Identifier::print(){
        std::cout << "Identifier(" << name << ")" << std::endl;
    }
    void Integer::print(){
        std::cout << "Integer(" << value << ")" << std::endl;
    }
    void Plus::print(){
        std::cout << "Plus" << std::endl;
    }
    void Hyphen::print(){
        std::cout << "Hyphen" << std::endl;
    }
    void Asterisk::print(){
        std::cout << "Asterisk" << std::endl;
    }
    void Slash::print(){
        std::cout << "Slash" << std::endl;
    }
}
