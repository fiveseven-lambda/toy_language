#include <iostream>
#include <boost/safe_numerics/safe_integer.hpp>

#include "token.hpp"
#include "error.hpp"

namespace token {
    Token::~Token() = default;
    Identifier::Identifier(std::string name): name(std::move(name)) {}
    Integer::Integer(std::string value): value(std::move(value)) {}

    std::optional<std::string> Token::identifier(){ return std::nullopt; }
    std::optional<std::string> Identifier::identifier(){ return std::move(name); }

    using safe_i32 = boost::safe_numerics::safe<std::int32_t>;
    std::optional<std::int32_t> Token::positive_integer(){
        return std::nullopt;
    }
    std::optional<std::int32_t> Integer::positive_integer(){
        safe_i32 ret(0);
        constexpr safe_i32 base(10);
        try{
            for(char c : value) ret = ret * base + safe_i32(c - '0');
        }catch(std::exception &e){
            throw error::make<error::InvalidIntegerLiteral>(e, pos);
        }
        return ret;
    }
    std::optional<std::int32_t> Token::negative_integer(){
        return std::nullopt;
    }
    std::optional<std::int32_t> Integer::negative_integer(){
        safe_i32 ret(0);
        constexpr safe_i32 base(10);
        try{
            for(char c : value) ret = ret * base - safe_i32(c - '0');
        }catch(std::exception &e){
            throw error::make<error::InvalidIntegerLiteral>(e, pos);
        }
        return ret;
    }

    std::optional<syntax::UnaryOperator> Token::prefix(){ return std::nullopt; }
    std::optional<syntax::UnaryOperator> Plus::prefix(){ return syntax::UnaryOperator::Plus; }
    std::optional<syntax::UnaryOperator> Hyphen::prefix(){ return syntax::UnaryOperator::Minus; }
    std::optional<syntax::UnaryOperator> Tilde::prefix(){ return syntax::UnaryOperator::BitNot; }
    std::optional<syntax::UnaryOperator> Exclamation::prefix(){ return syntax::UnaryOperator::LogicalNot; }
}

#define define_debug_print(name) \
    void name::debug_print() const { \
        std::cout << #name << std::endl; \
    }

namespace token {
    void Identifier::debug_print() const {
        std::cout << "Identifier(" << name << ")" << std::endl;
    }
    void Integer::debug_print() const {
        std::cout << "Integer(" << value << ")" << std::endl;
    }
    define_debug_print(Plus)
    define_debug_print(PlusEqual)
    define_debug_print(Hyphen)
    define_debug_print(HyphenEqual)
    define_debug_print(Asterisk)
    define_debug_print(AsteriskEqual)
    define_debug_print(Slash)
    define_debug_print(SlashEqual)
    define_debug_print(Percent)
    define_debug_print(PercentEqual)
    define_debug_print(Ampersand)
    define_debug_print(AmpersandEqual)
    define_debug_print(DoubleAmpersand)
    define_debug_print(Bar)
    define_debug_print(BarEqual)
    define_debug_print(DoubleBar)
    define_debug_print(Circumflex)
    define_debug_print(CircumflexEqual)
    define_debug_print(Tilde)
    define_debug_print(Equal)
    define_debug_print(DoubleEqual)
    define_debug_print(Exclamation)
    define_debug_print(ExclamationEqual)
    define_debug_print(Less)
    define_debug_print(LessEqual)
    define_debug_print(DoubleLess)
    define_debug_print(DoubleLessEqual)
    define_debug_print(Greater)
    define_debug_print(GreaterEqual)
    define_debug_print(DoubleGreater)
    define_debug_print(DoubleGreaterEqual)
    define_debug_print(OpeningParenthesis)
    define_debug_print(ClosingParenthesis)
    define_debug_print(OpeningBrace)
    define_debug_print(ClosingBrace)
    define_debug_print(OpeningBracket)
    define_debug_print(ClosingBracket)
    define_debug_print(Dot)
    define_debug_print(Colon)
    define_debug_print(Semicolon)
    define_debug_print(Comma)
}
