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

    std::optional<syntax::BinaryOperator> Token::binary_operator(){ return std::nullopt; }
    std::optional<syntax::BinaryOperator> Plus::binary_operator(){ return syntax::BinaryOperator::Add; }
    std::optional<syntax::BinaryOperator> PlusEqual::binary_operator(){ return syntax::BinaryOperator::AddAssign; }
    std::optional<syntax::BinaryOperator> Hyphen::binary_operator(){ return syntax::BinaryOperator::Sub; }
    std::optional<syntax::BinaryOperator> HyphenEqual::binary_operator(){ return syntax::BinaryOperator::SubAssign; }
    std::optional<syntax::BinaryOperator> Asterisk::binary_operator(){ return syntax::BinaryOperator::Mul; }
    std::optional<syntax::BinaryOperator> AsteriskEqual::binary_operator(){ return syntax::BinaryOperator::MulAssign; }
    std::optional<syntax::BinaryOperator> Slash::binary_operator(){ return syntax::BinaryOperator::Div; }
    std::optional<syntax::BinaryOperator> SlashEqual::binary_operator(){ return syntax::BinaryOperator::DivAssign; }
    std::optional<syntax::BinaryOperator> Percent::binary_operator(){ return syntax::BinaryOperator::Rem; }
    std::optional<syntax::BinaryOperator> PercentEqual::binary_operator(){ return syntax::BinaryOperator::RemAssign; }
    std::optional<syntax::BinaryOperator> Ampersand::binary_operator(){ return syntax::BinaryOperator::BitAnd; }
    std::optional<syntax::BinaryOperator> AmpersandEqual::binary_operator(){ return syntax::BinaryOperator::BitAndAssign; }
    std::optional<syntax::BinaryOperator> DoubleAmpersand::binary_operator(){ return syntax::BinaryOperator::LogicalAnd; }
    std::optional<syntax::BinaryOperator> Bar::binary_operator(){ return syntax::BinaryOperator::BitOr; }
    std::optional<syntax::BinaryOperator> BarEqual::binary_operator(){ return syntax::BinaryOperator::BitOrAssign; }
    std::optional<syntax::BinaryOperator> DoubleBar::binary_operator(){ return syntax::BinaryOperator::LogicalOr; }
    std::optional<syntax::BinaryOperator> Circumflex::binary_operator(){ return syntax::BinaryOperator::BitXor; }
    std::optional<syntax::BinaryOperator> CircumflexEqual::binary_operator(){ return syntax::BinaryOperator::BitXorAssign; }
    std::optional<syntax::BinaryOperator> Equal::binary_operator(){ return syntax::BinaryOperator::Assign; }
    std::optional<syntax::BinaryOperator> DoubleEqual::binary_operator(){ return syntax::BinaryOperator::Equal; }
    std::optional<syntax::BinaryOperator> ExclamationEqual::binary_operator(){ return syntax::BinaryOperator::NotEqual; }
    std::optional<syntax::BinaryOperator> Less::binary_operator(){ return syntax::BinaryOperator::Less; }
    std::optional<syntax::BinaryOperator> LessEqual::binary_operator(){ return syntax::BinaryOperator::LessEqual; }
    std::optional<syntax::BinaryOperator> DoubleLess::binary_operator(){ return syntax::BinaryOperator::LeftShift; }
    std::optional<syntax::BinaryOperator> DoubleLessEqual::binary_operator(){ return syntax::BinaryOperator::LeftShiftAssign; }
    std::optional<syntax::BinaryOperator> Greater::binary_operator(){ return syntax::BinaryOperator::Greater; }
    std::optional<syntax::BinaryOperator> GreaterEqual::binary_operator(){ return syntax::BinaryOperator::GreaterEqual; }
    std::optional<syntax::BinaryOperator> DoubleGreater::binary_operator(){ return syntax::BinaryOperator::RightShift; }
    std::optional<syntax::BinaryOperator> DoubleGreaterEqual::binary_operator(){ return syntax::BinaryOperator::RightShiftAssign; }

    bool Token::is_opening_parenthesis(){ return false; }
    bool OpeningParenthesis::is_opening_parenthesis(){ return true; }
    bool Token::is_closing_parenthesis(){ return false; }
    bool ClosingParenthesis::is_closing_parenthesis(){ return true; }
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
