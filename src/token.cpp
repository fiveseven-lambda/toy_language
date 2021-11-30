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

    std::optional<expression::UnaryOperator> Token::prefix(){ return std::nullopt; }
    std::optional<expression::UnaryOperator> Plus::prefix(){ return expression::UnaryOperator::Plus; }
    std::optional<expression::UnaryOperator> Hyphen::prefix(){ return expression::UnaryOperator::Minus; }
    std::optional<expression::UnaryOperator> Tilde::prefix(){ return expression::UnaryOperator::BitNot; }
    std::optional<expression::UnaryOperator> Exclamation::prefix(){ return expression::UnaryOperator::LogicalNot; }

    std::optional<expression::BinaryOperator> Token::binary_operator(){ return std::nullopt; }
    std::optional<expression::BinaryOperator> Plus::binary_operator(){ return expression::BinaryOperator::Add; }
    std::optional<expression::BinaryOperator> PlusEqual::binary_operator(){ return expression::BinaryOperator::AddAssign; }
    std::optional<expression::BinaryOperator> Hyphen::binary_operator(){ return expression::BinaryOperator::Sub; }
    std::optional<expression::BinaryOperator> HyphenEqual::binary_operator(){ return expression::BinaryOperator::SubAssign; }
    std::optional<expression::BinaryOperator> Asterisk::binary_operator(){ return expression::BinaryOperator::Mul; }
    std::optional<expression::BinaryOperator> AsteriskEqual::binary_operator(){ return expression::BinaryOperator::MulAssign; }
    std::optional<expression::BinaryOperator> Slash::binary_operator(){ return expression::BinaryOperator::Div; }
    std::optional<expression::BinaryOperator> SlashEqual::binary_operator(){ return expression::BinaryOperator::DivAssign; }
    std::optional<expression::BinaryOperator> Percent::binary_operator(){ return expression::BinaryOperator::Rem; }
    std::optional<expression::BinaryOperator> PercentEqual::binary_operator(){ return expression::BinaryOperator::RemAssign; }
    std::optional<expression::BinaryOperator> Ampersand::binary_operator(){ return expression::BinaryOperator::BitAnd; }
    std::optional<expression::BinaryOperator> AmpersandEqual::binary_operator(){ return expression::BinaryOperator::BitAndAssign; }
    std::optional<expression::BinaryOperator> DoubleAmpersand::binary_operator(){ return expression::BinaryOperator::LogicalAnd; }
    std::optional<expression::BinaryOperator> Bar::binary_operator(){ return expression::BinaryOperator::BitOr; }
    std::optional<expression::BinaryOperator> BarEqual::binary_operator(){ return expression::BinaryOperator::BitOrAssign; }
    std::optional<expression::BinaryOperator> DoubleBar::binary_operator(){ return expression::BinaryOperator::LogicalOr; }
    std::optional<expression::BinaryOperator> Circumflex::binary_operator(){ return expression::BinaryOperator::BitXor; }
    std::optional<expression::BinaryOperator> CircumflexEqual::binary_operator(){ return expression::BinaryOperator::BitXorAssign; }
    std::optional<expression::BinaryOperator> Equal::binary_operator(){ return expression::BinaryOperator::Assign; }
    std::optional<expression::BinaryOperator> DoubleEqual::binary_operator(){ return expression::BinaryOperator::Equal; }
    std::optional<expression::BinaryOperator> ExclamationEqual::binary_operator(){ return expression::BinaryOperator::NotEqual; }
    std::optional<expression::BinaryOperator> Less::binary_operator(){ return expression::BinaryOperator::Less; }
    std::optional<expression::BinaryOperator> LessEqual::binary_operator(){ return expression::BinaryOperator::LessEqual; }
    std::optional<expression::BinaryOperator> DoubleLess::binary_operator(){ return expression::BinaryOperator::LeftShift; }
    std::optional<expression::BinaryOperator> DoubleLessEqual::binary_operator(){ return expression::BinaryOperator::LeftShiftAssign; }
    std::optional<expression::BinaryOperator> Greater::binary_operator(){ return expression::BinaryOperator::Greater; }
    std::optional<expression::BinaryOperator> GreaterEqual::binary_operator(){ return expression::BinaryOperator::GreaterEqual; }
    std::optional<expression::BinaryOperator> DoubleGreater::binary_operator(){ return expression::BinaryOperator::RightShift; }
    std::optional<expression::BinaryOperator> DoubleGreaterEqual::binary_operator(){ return expression::BinaryOperator::RightShiftAssign; }

    bool Token::is_opening_parenthesis(){ return false; }
    bool OpeningParenthesis::is_opening_parenthesis(){ return true; }
    bool Token::is_closing_parenthesis(){ return false; }
    bool ClosingParenthesis::is_closing_parenthesis(){ return true; }
    bool Token::is_opening_brace(){ return false; }
    bool OpeningBrace::is_opening_brace(){ return true; }
    bool Token::is_closing_brace(){ return false; }
    bool ClosingBrace::is_closing_brace(){ return true; }
    bool Token::is_opening_bracket(){ return false; }
    bool OpeningBracket::is_opening_bracket(){ return true; }
    bool Token::is_closing_bracket(){ return false; }
    bool ClosingBracket::is_closing_bracket(){ return true; }
    bool Token::is_dot(){ return false; }
    bool Dot::is_dot(){ return true; }
    bool Token::is_comma(){ return false; }
    bool Comma::is_comma(){ return true; }
    bool Token::is_semicolon(){ return false; }
    bool Semicolon::is_semicolon(){ return true; }
    bool Token::is_colon(){ return false; }
    bool Colon::is_colon(){ return true; }
    bool Token::is_equal(){ return false; }
    bool Equal::is_equal(){ return true; }
}
