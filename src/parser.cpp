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
 * @brief `<Sentence>` をパースする．
 * @code
 * <Sentence> ::= <Expression>? `;`
 *              | <Identifier> `:` <Type>? ( `=` <Expression> )? `;`
 *              | `{` <Sentence>* `}`
 *              | `if` `(` <Expression> `)` <Sentence> ( `else` <Sentence> )?
 *              | `while` `(` <Expression> `)` <Sentence>
 * @endcode
 * @retval nullptr EOF に達した．
 * @throw error::NoIdentifierBeforeColon 宣言において `:` の前に `<Identifier>` 以外の `<Expression>` か ε があった
 * @throw error::NoSemicolonAfterDeclaration 宣言において `;` の代わりに EOF か別のトークンがあった
 * @throw error::NoExpressionAfterOperator 宣言において `=` の後に式が来なかった（`parse_expression()` が ε を返した）
 * @throw error::NoSemicolonAfterExpression ``<Sentence> ::= <Expression>? `;` `` において `<Expression>` はあったが `;` の代わりに EOF か別のトークンがあった
 * @throw error::NoClosingBrace `{` で開始したブロックが終わることなく EOF に達した
 * @throw error::NoParenthesisAfterKeyword `if`，`while` の後に開き丸括弧 `(` が来なかった（EOF か予期せぬトークン）
 * @throw error::NoClosingParenthesis `if`，`while` の条件節が終わることなく EOF に達した
 * @throw error::UnexpectedTokenInParenthesis `if`，`while` の条件節中に予期せぬトークンが現れた
 * @throw error::EmptyCondition `if`，`while` の条件節が空の `()` だった
 * @throw error::UnexpectedEOFInControlStatement `if()`，`while()`，`else` の後に文が無く，EOF に達した
 * @throw error::UnexpectedTokenAtSentence 先頭のトークンが `<Sentence>` を生成するいずれでもなかった
 */
std::unique_ptr<sentence::Sentence> parse_sentence(Lexer &lexer){
    auto expression = parse_expression(lexer);
    auto token = lexer.next();
    if(token && token->is_semicolon()){
        auto pos = expression ? expression->pos + token->pos : std::move(token->pos);
        auto ret = std::make_unique<sentence::Expression>(std::move(expression));
        ret->pos = std::move(pos);
        return ret;
    }else if(token && token->is_colon()){
        if(expression) if(auto identifier = expression->identifier()){
            auto pos = expression->pos + token->pos;
            auto type = parse_type(lexer);
            if(type) pos += type->pos;
            auto equal_or_semicolon = lexer.next();
            if(!equal_or_semicolon) throw error::make<error::NoSemicolonAfterDeclaration>(std::nullopt, std::move(pos));
            std::unique_ptr<expression::Expression> right_side;
            if(equal_or_semicolon->is_semicolon()){
                pos += equal_or_semicolon->pos;
            }else if(equal_or_semicolon->is_equal()){
                right_side = parse_expression(lexer);
                if(!right_side) throw error::make<error::NoExpressionAfterOperator>(std::move(equal_or_semicolon->pos));
                pos += right_side->pos;
                auto semicolon = lexer.next();
                if(semicolon && semicolon->is_semicolon()){
                    pos += semicolon->pos;
                }else{
                    std::optional<pos::Range> pos_not_semicolon;
                    if(semicolon) pos_not_semicolon = std::move(semicolon->pos);
                    throw error::make<error::NoSemicolonAfterDeclaration>(std::move(pos_not_semicolon), std::move(pos));
                }
            }else throw error::make<error::NoSemicolonAfterDeclaration>(std::move(equal_or_semicolon->pos), std::move(pos));
            auto ret = std::make_unique<sentence::Declaration>(std::move(identifier.value()), std::move(type), std::move(right_side));
            ret->pos = std::move(pos);
            return ret;
        }
        // コロンの前が識別子ではなかった
        std::optional<pos::Range> pos;
        if(expression) pos = std::move(expression->pos);
        throw error::make<error::NoIdentifierBeforeColon>(std::move(pos), std::move(token->pos));
    }else if(expression){
        // 式の終わりにセミコロンがないまま EOF
        std::optional<pos::Range> pos;
        if(token) pos = std::move(token->pos);
        throw error::make<error::NoSemicolonAfterExpression>(std::move(pos), std::move(expression->pos));
    }else if(token){
        if(token->is_opening_brace()){
            // ブロックの開始
            std::vector<std::unique_ptr<sentence::Sentence>> sentences;
            while(true){
                auto &token_ref = lexer.peek();
                if(!token_ref) throw error::make<error::NoClosingBrace>(std::move(token->pos));
                if(token_ref->is_closing_brace()){
                    auto ret = std::make_unique<sentence::Block>(std::move(sentences));
                    ret->pos = token->pos + lexer.next()->pos;
                    return ret;
                }
                // token_ref が nullptr でないので EOF ではない，よって parse_sentence() は nullptr を返さない
                sentences.push_back(parse_sentence(lexer));
            }
        }
        if(auto keyword = token->keyword()){
            if(keyword.value() == token::Keyword::If || keyword.value() == token::Keyword::Else){
                auto pos = std::move(token->pos);
                auto open = lexer.next();
                if(!open) throw error::make<error::NoParenthesisAfterKeyword>(std::nullopt, std::move(token->pos));
                if(!open->is_opening_parenthesis()) throw error::make<error::NoParenthesisAfterKeyword>(std::move(open->pos), std::move(token->pos));
                auto condition = parse_expression(lexer);
                auto close = lexer.next();
                if(!close) throw error::make<error::NoClosingParenthesis>(std::move(open->pos));
                if(!close->is_closing_parenthesis()) throw error::make<error::UnexpectedTokenInParenthesis>(std::move(close->pos), std::move(open->pos));
                if(!condition) throw error::make<error::EmptyCondition>(std::move(open->pos), std::move(close->pos));
                auto sentence = parse_sentence(lexer);
                if(!sentence) throw error::make<error::UnexpectedEOFInControlStatement>(pos + close->pos);
                if(keyword.value() == token::Keyword::If){
                    if(auto &else_ref = lexer.peek()){
                        if(auto keyword_else = else_ref->keyword()){
                            if(keyword_else.value() == token::Keyword::Else){
                                auto pos_else = std::move(lexer.next()->pos);
                                auto else_clause = parse_sentence(lexer);
                                if(!else_clause) throw error::make<error::UnexpectedEOFInControlStatement>(pos + pos_else);
                                pos += else_clause->pos;
                                auto ret = std::make_unique<sentence::If>(std::move(condition), std::move(sentence), std::move(else_clause));
                                ret->pos = std::move(pos);
                                return ret;
                            }
                        }
                    }
                }
                pos += sentence->pos;
                std::unique_ptr<sentence::Sentence> ret;
                if(keyword.value() == token::Keyword::If){
                    ret = std::make_unique<sentence::If>(std::move(condition), std::move(sentence), nullptr);
                }else{
                    ret = std::make_unique<sentence::While>(std::move(condition), std::move(sentence));
                }
                ret->pos = std::move(pos);
                return ret;
            }
        }
        throw error::make<error::UnexpectedTokenAtSentence>(std::move(token->pos));
    }else{
        // 正常に EOF に達した
        return nullptr;
    }
}
