/**
 * @file parser.cpp
 */
#include "parser.hpp"

#include "error.hpp"

static std::unique_ptr<type::Type> parse_type(Lexer &);

static std::unique_ptr<expression::Expression>
    parse_factor(Lexer &),
    parse_binary_operator(Lexer &, int),
    parse_expression(Lexer &);

static std::vector<std::unique_ptr<expression::Expression>> parse_list(Lexer &);

/**
 * @brief `<Type> | ε` をパース
 */
static std::unique_ptr<type::Type> parse_type(Lexer &lexer){
    auto &token_ref = lexer.peek();
    if(!token_ref) return nullptr;
    if(auto type = token_ref->primitive_type()){
        type->pos = std::move(lexer.next()->pos);
        return type;
    }else{
        return nullptr;
    }
}

/**
 * @brief `<Factor> | ε` をパース
 * @code
 * <Invocation> ::= <Identifier>
 *                | <Integer>
 *                | `(` <Expression> `)`
 *                | <Invocation> `(` <List> `)`
 * <Factor> ::= <Invocation>
 *            | <UnaryOperator> <Factor>
 * @endcode
 * - 先頭が前置単項演算子なら `<Factor> ::= <UnaryOperator> <Factor>`
 * - 先頭が識別子なら `<Factor> ::= <Invocation> ::= <Identifier>`
 * - 先頭が整数リテラルなら `<Factor> ::= <Invocation> ::= <Integer>`
 * - 先頭が開き括弧 `(` なら ``<Factor> ::= <Invocation> ::= `(` <Expression> `)` ``．
 *   このとき `parse_expression()` を用いるのでいったん `` `(` ( <Expression> | ε ) `)` `` をパースしてから ε でないことをチェックする
 * - `<Invocation>` の後に開き括弧 `(` が続いていたら ``<Invocation> ::= <Invocation> `(` <List> `)` ``
 * - それ以外なら ε
 *
 * @bug 前置単項演算子 `-` の直後に整数リテラルが続くとき，整数リテラルに対して `token::Token::negative_integer()` を呼び出して 1 つの `expression::Integer` としてしまうことで，`-2147483648` のパースを可能にしている．これは関数呼び出し `()` の優先順位が前置演算子 `-` より高いことに反していて，`-1()` という式が `-1` に対する関数呼び出しとなってしまっている．
 *
 * @retval nullptr ε
 * @throws error::UnexpectedEOFAfterPrefix `<Factor> ::= <UnaryOperator> <Factor>` の還元で `<Factor>` の代わりに EOF があった
 * @throws error::UnexpectedTokenAfterPrefix `<Factor> ::= <UnaryOperator> <Factor>` の還元で `<Factor>` の代わりに別のトークンがあった
 * @throws error::NoClosingParenthesis ``<Invocation> ::= `(` ( <Expression> | ε ) `)` `` あるいは ``<Invocation> ::= <Invocation> `(` <List> `)` `` の還元で `)` の代わりに EOF があった
 * @throws error::UnexpectedTokenInParenthesis ``<Invocation> ::= `(` ( <Expression> | ε ) `)` `` あるいは ``<Invocation> ::= <Invocation> `(` <List> `)` `` の還元で `)` の代わりに別のトークンがあった
 * @throws error::EmptyParenthesis ``<Invocation> ::= `(` <Expression> `)` `` の還元で `<Expression>` の代わりに ε があった
 */
static std::unique_ptr<expression::Expression> parse_factor(Lexer &lexer){
    std::unique_ptr<expression::Expression> ret;
    {
        auto &token_ref = lexer.peek();
        if(!token_ref) return nullptr;

        pos::Range pos;
        if(auto name = token_ref->identifier()){
            pos = std::move(lexer.next()->pos);
            ret = std::make_unique<expression::Identifier>(std::move(name).value());
        }else if(auto value = token_ref->positive_integer()){
            pos = std::move(lexer.next()->pos);
            ret = std::make_unique<expression::Integer>(value.value());
        }else if(auto prefix = token_ref->prefix()){
            pos = std::move(lexer.next()->pos);
            auto &operand_ref = lexer.peek();
            if(!operand_ref) throw error::make<error::UnexpectedEOFAfterPrefix>(std::move(pos));
            if(
                prefix.value() == expression::UnaryOperator::Minus
                && (value = operand_ref->negative_integer())
            ){
                pos += lexer.next()->pos;
                ret = std::make_unique<expression::Integer>(value.value());
            }else{
                auto operand = parse_factor(lexer);
                if(!operand) throw error::make<error::UnexpectedTokenAfterPrefix>(std::move(operand_ref->pos), std::move(pos));
                pos += operand->pos;
                ret = std::make_unique<expression::UnaryOperation>(prefix.value(), std::move(operand));
            }
        }else if(token_ref->is_opening_parenthesis()){
            pos = std::move(lexer.next()->pos);
            auto expression = parse_expression(lexer);
            auto close = lexer.next();
            if(!close) throw error::make<error::NoClosingParenthesis>(std::move(pos));
            if(!close->is_closing_parenthesis()) throw error::make<error::UnexpectedTokenInParenthesis>(std::move(close->pos), std::move(pos));
            if(!expression) throw error::make<error::EmptyParenthesis>(std::move(pos), std::move(close->pos));
            ret = std::make_unique<expression::Group>(std::move(expression));
            pos += close->pos;
        }else{
            return nullptr;
        }
        ret->pos = std::move(pos);
    }
    while(true){
        auto &token_ref = lexer.peek();
        if(token_ref && token_ref->is_opening_parenthesis()){
            auto pos_open = std::move(lexer.next()->pos);
            auto arguments = parse_list(lexer);
            auto close = lexer.next();
            if(!close) throw error::make<error::NoClosingParenthesis>(std::move(pos_open));
            if(!close->is_closing_parenthesis()) throw error::make<error::UnexpectedTokenInParenthesis>(std::move(close->pos), std::move(pos_open));
            pos::Range pos = ret->pos + close->pos;
            ret = std::make_unique<expression::Invocation>(std::move(ret), std::move(arguments));
            ret->pos = std::move(pos);
        }else{
            return ret;
        }
    }
}

//! 2 項演算子の優先順位
enum Precedence{
    //! 代入演算子 `=` と複合代入演算子
    AssignPrecedence,
    //! `||`
    LogicalOrPrecedence,
    //! `&&`
    LogicalAndPrecedence,
    //! 比較演算子 `==` `!=` `<` `<=` `>` `>=`
    ComparisonPrecedence,
    //! `|`
    BitOrPrecedence,
    //! `^`
    BitXorPrecedence,
    //! `&`
    BitAndPrecedence,
    //! `<<` `>>`
    ShiftPrecedence,
    //! `+` `-`
    AddSubPrecedence,
    //! `*` `/` `%`
    MulDivRemPrecedence,
    //! 番兵
    MaxPrecedence
};

//! 与えられた `expression::BinaryOperator` の優先順位 `Precedence` を返す
static Precedence precedence(expression::BinaryOperator binary_operator){
    switch(binary_operator){
        case expression::BinaryOperator::Mul:
        case expression::BinaryOperator::Div:
        case expression::BinaryOperator::Rem:
            return MulDivRemPrecedence;
        case expression::BinaryOperator::Add:
        case expression::BinaryOperator::Sub:
            return AddSubPrecedence;
        case expression::BinaryOperator::LeftShift:
        case expression::BinaryOperator::RightShift:
            return ShiftPrecedence;
        case expression::BinaryOperator::BitAnd:
            return BitAndPrecedence;
        case expression::BinaryOperator::BitXor:
            return BitXorPrecedence;
        case expression::BinaryOperator::BitOr:
            return BitOrPrecedence;
        case expression::BinaryOperator::Equal:
        case expression::BinaryOperator::NotEqual:
        case expression::BinaryOperator::Less:
        case expression::BinaryOperator::Greater:
        case expression::BinaryOperator::LessEqual:
        case expression::BinaryOperator::GreaterEqual:
            return ComparisonPrecedence;
        case expression::BinaryOperator::LogicalAnd:
            return LogicalAndPrecedence;
        case expression::BinaryOperator::LogicalOr:
            return LogicalOrPrecedence;
        case expression::BinaryOperator::Assign:
        case expression::BinaryOperator::MulAssign:
        case expression::BinaryOperator::DivAssign:
        case expression::BinaryOperator::RemAssign:
        case expression::BinaryOperator::AddAssign:
        case expression::BinaryOperator::SubAssign:
        case expression::BinaryOperator::LeftShiftAssign:
        case expression::BinaryOperator::RightShiftAssign:
        case expression::BinaryOperator::BitAndAssign:
        case expression::BinaryOperator::BitXorAssign:
        case expression::BinaryOperator::BitOrAssign:
            return AssignPrecedence;
    }
}

//! 2 項演算子の結合の向き
enum class Associativity{
    //! 左結合
    LeftToRight,
    //! 右結合
    RightToLeft
};

/**
 * @brief 与えられた優先順位における 2 項演算子の結合の向きを返す
 * @param precedence 優先順位（`parse_binary_operator()` の実装の都合により `Precedence` ではなく `int`）
 */
static Associativity associativity(int precedence){
    if(precedence == AssignPrecedence) return Associativity::RightToLeft;
    else return Associativity::LeftToRight;
}

/**
 * @brief `<Expression> | ε` をパース
 * @code
 * <Expression> ::= <Expression> <BinaryOperator> <Expression>
 * @endcode
 * @param current_precedence 今見ている優先順位．再帰呼び出しのネスト内側ほど大きい
 * @retval nullptr ε
 * @throw error::NoExpressionAfterOperator 2 項演算子の右オペランドが空だった（EOF に達したか，別のトークンがあったか）
 */
static std::unique_ptr<expression::Expression> parse_binary_operator(Lexer &lexer, int current_precedence){
    if(current_precedence == MaxPrecedence){
        return parse_factor(lexer);
    }
    auto left = parse_binary_operator(lexer, current_precedence + 1);
    if(!left) return nullptr;
    bool left_to_right = associativity(current_precedence) == Associativity::LeftToRight;
    while(true){
        // ここで left は ε でない（Expression）
        auto &token = lexer.peek();
        if(!token) return left;
        auto binary_operator = token->infix();
        if(binary_operator && precedence(binary_operator.value()) == current_precedence){
            auto &operator_pos = lexer.next()->pos;
            auto right = parse_binary_operator(lexer, current_precedence + left_to_right);
            if(!right) throw error::make<error::NoExpressionAfterOperator>(std::move(operator_pos));
            pos::Range pos = left->pos + right->pos;
            left = std::make_unique<expression::BinaryOperation>(binary_operator.value(), std::move(left), std::move(right));
            left->pos = std::move(pos);
            if(left_to_right) continue;
        }
        return left;
    }
}

/**
 * @brief `<Expression> | ε` をパース
 *
 * 実は `parse_binary_operator()` を `current_precedence = 0` で呼び出しているだけ
 */
std::unique_ptr<expression::Expression> parse_expression(Lexer &lexer){
    return parse_binary_operator(lexer, 0);
}

/**
 * @brief `<List>` をパース
 * @code
 * <List> ::= ( <Expression> `,` )* <Expression>?
 * @endcode
 * @throw error::EmptyArgument コンマ `,` の前が `<Expression>` でなく ε だった
 */
static std::vector<std::unique_ptr<expression::Expression>> parse_list(Lexer &lexer){
    std::vector<std::unique_ptr<expression::Expression>> ret;
    while(true){
        auto argument = parse_expression(lexer);
        // ここで argument は nullptr の可能性がある
        auto &token = lexer.peek();
        if(token && token->is_comma()){
            auto pos_comma = std::move(lexer.next()->pos);
            if(!argument) throw error::make<error::EmptyArgument>(std::move(pos_comma));
            ret.push_back(std::move(argument));
        }else{
            if(argument) ret.push_back(std::move(argument));
            return ret;
        }
    }
}

/**
 * @brief 入力をパースして文を返す．
 */
std::unique_ptr<sentence::Sentence> parse_sentence(Lexer &lexer){
    auto expression = parse_expression(lexer);
    auto &token_ref = lexer.peek();
    if(!token_ref){
        if(expression){
            // 式の終わりにセミコロンがないまま EOF
            // 本当はエラー
            return nullptr;
        }else{
            // 正常に EOF に達した
            return nullptr;
        }
    }
    if(token_ref->is_semicolon()){
        pos::Range pos;
        if(expression){
            pos = expression->pos.clone();
            pos += lexer.next()->pos;
        }else{
            pos = std::move(lexer.next()->pos);
        }
        auto ret = std::make_unique<sentence::Expression>(std::move(expression));
        ret->pos = std::move(pos);
        return ret;
    }else if(token_ref->is_colon()){
        if(auto identifier = expression->identifier()){
            auto pos = std::move(expression->pos);
            lexer.next(); // コロン自体
            auto type = parse_type(lexer);

            auto equal_or_semicolon = lexer.next();
            pos::Range end_pos;

            std::unique_ptr<expression::Expression> right_side;
            if(equal_or_semicolon->is_semicolon()){
                right_side = nullptr;
                end_pos = std::move(equal_or_semicolon->pos);
            }else if(equal_or_semicolon->is_equal()){
                right_side = parse_expression(lexer);
                auto semicolon = lexer.next();
                if(!semicolon->is_semicolon()){
                    // エラー
                    return nullptr;
                }
                end_pos = std::move(semicolon->pos);
            }else{
                // エラー
                return nullptr;
            }

            pos += end_pos;
            auto ret = std::make_unique<sentence::Declaration>(std::move(identifier.value()), std::move(type), std::move(right_side));
            ret->pos = std::move(pos);
            return ret;
        }else{
            // コロンの前が識別子ではない
            // 本当はエラー
            return nullptr;
        }
    }
    return nullptr;
}
