/**
 * @file token.cpp
 */
#include "token.hpp"

#include <boost/safe_numerics/safe_integer.hpp>

#include "error.hpp"

namespace token {
    Token::~Token() = default;
    //! @brief コンストラクタ
    Identifier::Identifier(std::string name): name(std::move(name)) {}
    //! @brief コンストラクタ
    Integer::Integer(std::string value): value(std::move(value)) {}

    /**
     * @brief キーワードを `token::Keyword` に変換する．
     * @retval std::nullopt キーワードではない．
     */
    std::optional<Keyword> Token::keyword(){ return std::nullopt; }
    std::optional<Keyword> Identifier::keyword(){
        if(name == "if") return Keyword::If;
        if(name == "else") return Keyword::Else;
        if(name == "while") return Keyword::While;
        return std::nullopt;
    }

    /**
     * @brief 識別子を `std::string` に変換する．
     * @retval std::nullopt 識別子ではない．
     */
    std::optional<std::string> Token::identifier(){ return std::nullopt; }
    std::optional<std::string> Identifier::identifier(){
        if(keyword()) return std::nullopt;
        return std::move(name);
    }

    /**
     * @brief プリミティブ型の名前を `type::Type` に変換する．
     * @retval nullptr プリミティブ型の名前ではない．
     */
    std::unique_ptr<type::Type> Token::primitive_type(){ return nullptr; }
    std::unique_ptr<type::Type> Identifier::primitive_type(){
        if(name == "integer"){
            return std::make_unique<type::Integer>();
        }else if(name == "boolean"){
            return std::make_unique<type::Boolean>();
        }else{
            return nullptr;
        }
    }

    /**
     * @brief 整数リテラルを整数値に変換する．
     * @retval std::nullopt 整数リテラルではない．
     * @throw error::InvalidIntegerLiteral 32 ビットにおさまらなかった．
     */
    std::optional<std::int32_t> Token::positive_integer(){ return std::nullopt; }
    std::optional<std::int32_t> Integer::positive_integer(){
        using safe_i32 = boost::safe_numerics::safe<std::int32_t>;
        safe_i32 ret(0);
        constexpr safe_i32 base(10);
        try{
            for(char c : value) ret = ret * base + safe_i32(c - '0');
        }catch(std::exception &e){
            throw error::make<error::InvalidIntegerLiteral>(e, std::move(pos));
        }
        return ret;
    }

    /**
     * @brief 整数リテラルに負号を付けて整数値に変換する．
     * @retval std::nullopt 整数リテラルではない．
     * @throw error::InvalidIntegerLiteral 32 ビットにおさまらなかった．
     */
    std::optional<std::int32_t> Token::negative_integer(){ return std::nullopt; }
    std::optional<std::int32_t> Integer::negative_integer(){
        using safe_i32 = boost::safe_numerics::safe<std::int32_t>;
        safe_i32 ret(0);
        constexpr safe_i32 base(10);
        try{
            for(char c : value) ret = ret * base - safe_i32(c - '0');
        }catch(std::exception &e){
            throw error::make<error::InvalidIntegerLiteral>(e, std::move(pos));
        }
        return ret;
    }

    /**
     * @brief 前置単項演算子を `expression::UnaryOperator` に変換する．
     * @retval std::nullopt 前置単項演算子ではない．
     */
    std::optional<expression::UnaryOperator> Token::prefix(){ return std::nullopt; }
    std::optional<expression::UnaryOperator> Plus::prefix(){ return expression::UnaryOperator::Plus; }
    std::optional<expression::UnaryOperator> Hyphen::prefix(){ return expression::UnaryOperator::Minus; }
    std::optional<expression::UnaryOperator> Tilde::prefix(){ return expression::UnaryOperator::BitNot; }
    std::optional<expression::UnaryOperator> Exclamation::prefix(){ return expression::UnaryOperator::LogicalNot; }

    /**
     * @brief 中置 2 項演算子を `expression::BinaryOperator` に変換する．
     * @retval std::nullopt 中置 2 項演算子ではない．
     */
    std::optional<expression::BinaryOperator> Token::infix(){ return std::nullopt; }
    std::optional<expression::BinaryOperator> Plus::infix(){ return expression::BinaryOperator::Add; }
    std::optional<expression::BinaryOperator> PlusEqual::infix(){ return expression::BinaryOperator::AddAssign; }
    std::optional<expression::BinaryOperator> Hyphen::infix(){ return expression::BinaryOperator::Sub; }
    std::optional<expression::BinaryOperator> HyphenEqual::infix(){ return expression::BinaryOperator::SubAssign; }
    std::optional<expression::BinaryOperator> Asterisk::infix(){ return expression::BinaryOperator::Mul; }
    std::optional<expression::BinaryOperator> AsteriskEqual::infix(){ return expression::BinaryOperator::MulAssign; }
    std::optional<expression::BinaryOperator> Slash::infix(){ return expression::BinaryOperator::Div; }
    std::optional<expression::BinaryOperator> SlashEqual::infix(){ return expression::BinaryOperator::DivAssign; }
    std::optional<expression::BinaryOperator> Percent::infix(){ return expression::BinaryOperator::Rem; }
    std::optional<expression::BinaryOperator> PercentEqual::infix(){ return expression::BinaryOperator::RemAssign; }
    std::optional<expression::BinaryOperator> Ampersand::infix(){ return expression::BinaryOperator::BitAnd; }
    std::optional<expression::BinaryOperator> AmpersandEqual::infix(){ return expression::BinaryOperator::BitAndAssign; }
    std::optional<expression::BinaryOperator> DoubleAmpersand::infix(){ return expression::BinaryOperator::LogicalAnd; }
    std::optional<expression::BinaryOperator> Bar::infix(){ return expression::BinaryOperator::BitOr; }
    std::optional<expression::BinaryOperator> BarEqual::infix(){ return expression::BinaryOperator::BitOrAssign; }
    std::optional<expression::BinaryOperator> DoubleBar::infix(){ return expression::BinaryOperator::LogicalOr; }
    std::optional<expression::BinaryOperator> Circumflex::infix(){ return expression::BinaryOperator::BitXor; }
    std::optional<expression::BinaryOperator> CircumflexEqual::infix(){ return expression::BinaryOperator::BitXorAssign; }
    std::optional<expression::BinaryOperator> Equal::infix(){ return expression::BinaryOperator::Assign; }
    std::optional<expression::BinaryOperator> DoubleEqual::infix(){ return expression::BinaryOperator::Equal; }
    std::optional<expression::BinaryOperator> ExclamationEqual::infix(){ return expression::BinaryOperator::NotEqual; }
    std::optional<expression::BinaryOperator> Less::infix(){ return expression::BinaryOperator::Less; }
    std::optional<expression::BinaryOperator> LessEqual::infix(){ return expression::BinaryOperator::LessEqual; }
    std::optional<expression::BinaryOperator> DoubleLess::infix(){ return expression::BinaryOperator::LeftShift; }
    std::optional<expression::BinaryOperator> DoubleLessEqual::infix(){ return expression::BinaryOperator::LeftShiftAssign; }
    std::optional<expression::BinaryOperator> Greater::infix(){ return expression::BinaryOperator::Greater; }
    std::optional<expression::BinaryOperator> GreaterEqual::infix(){ return expression::BinaryOperator::GreaterEqual; }
    std::optional<expression::BinaryOperator> DoubleGreater::infix(){ return expression::BinaryOperator::RightShift; }
    std::optional<expression::BinaryOperator> DoubleGreaterEqual::infix(){ return expression::BinaryOperator::RightShiftAssign; }

    /**
     * @retval true イコール `=`
     * @retval false それ以外
     */
    bool Token::is_equal() const { return false; }
    bool Equal::is_equal() const { return true; }

    /**
     * @retval true 開き丸括弧 `(`
     * @retval false それ以外
     */
    bool Token::is_opening_parenthesis() const { return false; }
    bool OpeningParenthesis::is_opening_parenthesis() const { return true; }

    /**
     * @retval true 閉じ丸括弧 `)`
     * @retval false それ以外
     */
    bool Token::is_closing_parenthesis() const { return false; }
    bool ClosingParenthesis::is_closing_parenthesis() const { return true; }

    /**
     * @retval true 開き波括弧 `{`
     * @retval false それ以外
     */
    bool Token::is_opening_brace() const { return false; }
    bool OpeningBrace::is_opening_brace() const { return true; }

    /**
     * @retval true 閉じ波括弧 `}`
     * @retval false それ以外
     */
    bool Token::is_closing_brace() const { return false; }
    bool ClosingBrace::is_closing_brace() const { return true; }

    /**
     * @retval true 開き四角括弧 `[`
     * @retval false それ以外
     */
    bool Token::is_opening_bracket() const { return false; }
    bool OpeningBracket::is_opening_bracket() const { return true; }

    /**
     * @retval true 閉じ四角括弧 `]`
     * @retval false それ以外
     */
    bool Token::is_closing_bracket() const { return false; }
    bool ClosingBracket::is_closing_bracket() const { return true; }

    /**
     * @retval true ドット `.`
     * @retval false それ以外
     */
    bool Token::is_dot() const { return false; }
    bool Dot::is_dot() const { return true; }

    /**
     * @retval true コロン `:`
     * @retval false それ以外
     */
    bool Token::is_colon() const { return false; }
    bool Colon::is_colon() const { return true; }

    /**
     * @retval true セミコロン `;`
     * @retval false それ以外
     */
    bool Token::is_semicolon() const { return false; }
    bool Semicolon::is_semicolon() const { return true; }

    /**
     * @retval true コンマ `,`
     * @retval false それ以外
     */
    bool Token::is_comma() const { return false; }
    bool Comma::is_comma() const { return true; }
}
