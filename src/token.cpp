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

    std::unique_ptr<type::Type> Token::type_name(){
        return nullptr;
    }
    std::unique_ptr<type::Type> Identifier::type_name(){
        std::unique_ptr<type::Type> ret;
        if(name == "integer"){
            ret = std::make_unique<type::Integer>();
        }else if(name == "boolean"){
            ret = std::make_unique<type::Boolean>();
        }else{
            return nullptr;
        }
        ret->pos = pos;
        return ret;
    }

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

    bool Token::is_opening_parenthesis(){ return false; }
    bool OpeningParenthesis::is_opening_parenthesis(){ return true; }
    bool Token::is_closing_parenthesis(){ return false; }
    bool ClosingParenthesis::is_closing_parenthesis(){ return true; }
    bool Token::is_comma(){ return false; }
    bool Comma::is_comma(){ return true; }
    bool Token::is_semicolon(){ return false; }
    bool Semicolon::is_semicolon(){ return true; }
    bool Token::is_colon(){ return false; }
    bool Colon::is_colon(){ return true; }
    bool Token::is_equal(){ return false; }
    bool Equal::is_equal(){ return true; }
}

namespace token {
    using expression::UnaryOperator;

    std::optional<UnaryOperator> Token::prefix(){ return std::nullopt; }
    std::optional<UnaryOperator> Plus::prefix(){ return UnaryOperator::Plus; }
    std::optional<UnaryOperator> Hyphen::prefix(){ return UnaryOperator::Minus; }
    std::optional<UnaryOperator> Tilde::prefix(){ return UnaryOperator::BitNot; }
    std::optional<UnaryOperator> Exclamation::prefix(){ return UnaryOperator::LogicalNot; }
}
namespace token {
    using expression::BinaryOperator;

    std::optional<BinaryOperator> Token::binary_operator(){ return std::nullopt; }
    std::optional<BinaryOperator> Plus::binary_operator(){ return BinaryOperator::Add; }
    std::optional<BinaryOperator> PlusEqual::binary_operator(){ return BinaryOperator::AddAssign; }
    std::optional<BinaryOperator> Hyphen::binary_operator(){ return BinaryOperator::Sub; }
    std::optional<BinaryOperator> HyphenEqual::binary_operator(){ return BinaryOperator::SubAssign; }
    std::optional<BinaryOperator> Asterisk::binary_operator(){ return BinaryOperator::Mul; }
    std::optional<BinaryOperator> AsteriskEqual::binary_operator(){ return BinaryOperator::MulAssign; }
    std::optional<BinaryOperator> Slash::binary_operator(){ return BinaryOperator::Div; }
    std::optional<BinaryOperator> SlashEqual::binary_operator(){ return BinaryOperator::DivAssign; }
    std::optional<BinaryOperator> Percent::binary_operator(){ return BinaryOperator::Rem; }
    std::optional<BinaryOperator> PercentEqual::binary_operator(){ return BinaryOperator::RemAssign; }
    std::optional<BinaryOperator> Ampersand::binary_operator(){ return BinaryOperator::BitAnd; }
    std::optional<BinaryOperator> AmpersandEqual::binary_operator(){ return BinaryOperator::BitAndAssign; }
    std::optional<BinaryOperator> DoubleAmpersand::binary_operator(){ return BinaryOperator::LogicalAnd; }
    std::optional<BinaryOperator> Bar::binary_operator(){ return BinaryOperator::BitOr; }
    std::optional<BinaryOperator> BarEqual::binary_operator(){ return BinaryOperator::BitOrAssign; }
    std::optional<BinaryOperator> DoubleBar::binary_operator(){ return BinaryOperator::LogicalOr; }
    std::optional<BinaryOperator> Circumflex::binary_operator(){ return BinaryOperator::BitXor; }
    std::optional<BinaryOperator> CircumflexEqual::binary_operator(){ return BinaryOperator::BitXorAssign; }
    std::optional<BinaryOperator> Equal::binary_operator(){ return BinaryOperator::Assign; }
    std::optional<BinaryOperator> DoubleEqual::binary_operator(){ return BinaryOperator::Equal; }
    std::optional<BinaryOperator> ExclamationEqual::binary_operator(){ return BinaryOperator::NotEqual; }
    std::optional<BinaryOperator> Less::binary_operator(){ return BinaryOperator::Less; }
    std::optional<BinaryOperator> LessEqual::binary_operator(){ return BinaryOperator::LessEqual; }
    std::optional<BinaryOperator> DoubleLess::binary_operator(){ return BinaryOperator::LeftShift; }
    std::optional<BinaryOperator> DoubleLessEqual::binary_operator(){ return BinaryOperator::LeftShiftAssign; }
    std::optional<BinaryOperator> Greater::binary_operator(){ return BinaryOperator::Greater; }
    std::optional<BinaryOperator> GreaterEqual::binary_operator(){ return BinaryOperator::GreaterEqual; }
    std::optional<BinaryOperator> DoubleGreater::binary_operator(){ return BinaryOperator::RightShift; }
    std::optional<BinaryOperator> DoubleGreaterEqual::binary_operator(){ return BinaryOperator::RightShiftAssign; }
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
