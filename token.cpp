
#include "token.hpp"

#include <boost/safe_numerics/safe_integer.hpp>
#include <iostream>

namespace token {
    Token::Token() = default;
    Token::~Token() = default;
    Identifier::Identifier(std::string name):
        name(std::move(name)) {}
    Integer::Integer(std::string value):
        value(std::move(value)) {}

    std::optional<std::string> Token::identifier(){ return std::nullopt; }
    std::optional<std::string> Identifier::identifier(){ return std::move(name); }

    using safe_i32 = boost::safe_numerics::safe<std::int32_t>;
    std::optional<std::int32_t> Token::positive_integer(){ return std::nullopt; }
    std::optional<std::int32_t> Integer::positive_integer(){
        safe_i32 ret(0);
        constexpr safe_i32 base(10);
        try{
            for(char c : value){
                ret = ret * base + safe_i32(c - '0');
            }
        }catch(std::exception &e){
            std::cerr << e.what() << std::endl;
            return -1;
        }
        return ret;
    }
    std::optional<std::int32_t> Token::negative_integer(){ return std::nullopt; }
    std::optional<std::int32_t> Integer::negative_integer(){
        safe_i32 ret(0);
        constexpr safe_i32 base(10);
        try{
            for(char c : value){
                ret = ret * base - safe_i32(c - '0');
            }
        }catch(std::exception &e){
            std::cerr << e.what() << std::endl;
            return -1;
        }
        return ret;
    }

    std::optional<syntax::UnaryOperator> Token::prefix(){ return std::nullopt; }
    std::optional<syntax::UnaryOperator> Plus::prefix(){ return syntax::UnaryOperator::Plus; }
    std::optional<syntax::UnaryOperator> Hyphen::prefix(){ return syntax::UnaryOperator::Minus; }

    std::optional<syntax::BinaryOperator> Token::binary_operator(){ return std::nullopt; }
    std::optional<syntax::BinaryOperator> Plus::binary_operator(){ return syntax::BinaryOperator::Add; }
    std::optional<syntax::BinaryOperator> Hyphen::binary_operator(){ return syntax::BinaryOperator::Sub; }
    std::optional<syntax::BinaryOperator> Asterisk::binary_operator(){ return syntax::BinaryOperator::Mul; }
    std::optional<syntax::BinaryOperator> Slash::binary_operator(){ return syntax::BinaryOperator::Div; }

    bool Token::is_opening_parenthesis(){ return false; }
    bool OpeningParenthesis::is_opening_parenthesis(){ return true; }
    bool Token::is_closing_parenthesis(){ return false; }
    bool ClosingParenthesis::is_closing_parenthesis(){ return true; }
}
