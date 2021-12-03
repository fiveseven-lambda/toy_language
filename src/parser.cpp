#include "parser.hpp"

#include "type.hpp"
#include "expression.hpp"
#include "error.hpp"

static std::unique_ptr<expression::Expression>
    parse_factor(Lexer &),
    parse_binary_operator(Lexer &, int),
    parse_expression(Lexer &);
static std::vector<std::unique_ptr<expression::Expression>> parse_arguments(Lexer &);
static std::shared_ptr<type::Type> parse_type(Lexer &);

std::unique_ptr<expression::Expression> parse_factor(Lexer &lexer){
    std::unique_ptr<expression::Expression> ret;
    {
        auto &token_ref = lexer.peek();
        if(!token_ref) return nullptr;

        pos::Range pos;
        if(auto name = token_ref->identifier()){
            pos = lexer.next()->pos;
            ret = std::make_unique<expression::Identifier>(std::move(name).value());
        }else if(auto value = token_ref->positive_integer()){
            pos = lexer.next()->pos;
            ret = std::make_unique<expression::Integer>(value.value());
        }else if(auto prefix = token_ref->prefix()){
            pos = lexer.next()->pos;
            if(prefix.value() == expression::UnaryOperator::Minus && (value = lexer.peek()->negative_integer())){
                pos += lexer.next()->pos;
                ret = std::make_unique<expression::Integer>(value.value());
            }else{
                auto operand = parse_factor(lexer);
                pos += operand->pos;
                ret = std::make_unique<expression::Unary>(prefix.value(), std::move(operand));
            }
        }else if(token_ref->is_opening_parenthesis()){
            pos = lexer.next()->pos;
            ret = parse_expression(lexer);
            if(!ret) throw error::make<error::EmptyParenthesis>(std::move(pos));
            auto close = lexer.next();
            if(!close) throw error::make<error::NoClosingParenthesis>(std::move(pos));
            if(!close->is_closing_parenthesis()) throw error::make<error::UnexpectedTokenInParenthesis>(std::move(close->pos), std::move(pos));
            pos += close->pos;
        }else{
            return nullptr;
        }
        ret->pos = pos;
    }
    while(true){
        auto &token_ref = lexer.peek();
        if(token_ref && token_ref->is_opening_parenthesis()){
            lexer.next();
            pos::Range pos = ret->pos;
            auto arguments = parse_arguments(lexer);
            auto close = lexer.next();
            if(!close) throw error::make<error::NoClosingParenthesis>(std::move(pos));
            if(!close->is_closing_parenthesis()) throw error::make<error::UnexpectedTokenInParenthesis>(std::move(close->pos), std::move(pos));
            ret = std::make_unique<expression::Invocation>(std::move(ret), std::move(arguments));
            pos += close->pos;
            ret->pos = pos;
        }else{
            return ret;
        }
    }
}

enum Precedence {
    AssignPrecedence,
    LogicalOrPrecedence,
    LogicalAndPrecedence,
    ComparisonPrecedence,
    BitOrPrecedence,
    BitXorPrecedence,
    BitAndPrecedence,
    ShiftPrecedence,
    AddSubPrecedence,
    MulDivRemPrecedence,
    MaxPrecedence
};
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
enum class Associativity {
    LeftToRight,
    RightToLeft
};
static Associativity associativity(int precedence){
    if(precedence == AssignPrecedence) return Associativity::RightToLeft;
    else return Associativity::LeftToRight;
}
std::unique_ptr<expression::Expression> parse_binary_operator(Lexer &lexer, int current_precedence){
    if(current_precedence == MaxPrecedence){
        return parse_factor(lexer);
    }
    auto left = parse_binary_operator(lexer, current_precedence + 1);
    if(!left) return nullptr;
    bool left_to_right = associativity(current_precedence) == Associativity::LeftToRight;
    while(true){
        auto &token = lexer.peek();
        if(!token) return left;
        auto binary_operator = token->binary_operator();
        if(binary_operator && precedence(binary_operator.value()) == current_precedence){
            auto operator_pos = lexer.next()->pos;
            auto right = parse_binary_operator(lexer, current_precedence + left_to_right);
            if(!right) throw error::make<error::NoExpressionAfterOperator>(std::move(operator_pos));
            pos::Range pos = left->pos;
            pos += right->pos;
            left = std::make_unique<expression::Binary>(binary_operator.value(), std::move(left), std::move(right));
            left->pos = pos;
            if(left_to_right) continue;
        }
        return left;
    }
}

std::unique_ptr<expression::Expression> parse_expression(Lexer &lexer){
    return parse_binary_operator(lexer, 0);
}

std::vector<std::unique_ptr<expression::Expression>> parse_arguments(Lexer &lexer){
    std::vector<std::unique_ptr<expression::Expression>> ret;
    while(true){
        auto argument = parse_expression(lexer);
        auto &token = lexer.peek();
        if(token && token->is_comma()){
            auto pos_comma = lexer.next()->pos;
            if(!argument) throw error::make<error::EmptyArgument>(std::move(pos_comma));
            ret.push_back(std::move(argument));
        }else{
            if(argument) ret.push_back(std::move(argument));
            return ret;
        }
    }
}

std::shared_ptr<type::Type> parse_type(Lexer &lexer){
    auto &token_ref = lexer.peek();
    if(!token_ref) return nullptr;
    auto type = token_ref->type_name();
    if(type) lexer.next();
    return type;
}

std::unique_ptr<sentence::Sentence> parse_sentence(Lexer &lexer){
    auto expression = parse_expression(lexer);
    auto next = lexer.next();

    if(!next){
        if(expression){
            // エラー
            return nullptr;
        }else{
            return nullptr;
        }
    }

    std::unique_ptr<sentence::Sentence> ret;
    pos::Range pos;
    if(next->is_semicolon()){
        if(expression){
            pos = expression->pos;
            pos += next->pos;
        }else{
            pos = next->pos;
        }
        ret = std::make_unique<sentence::Expression>(std::move(expression));
    }else if(next->is_colon()){
        if(!expression){
            // ほんとはエラー
            return nullptr;
        }
        pos = expression->pos;
        std::optional<std::string> identifier = expression->identifier();
        if(!identifier){
            // ほんとはエラー
            return nullptr;
        }
        auto type = parse_type(lexer);
        auto next2 = lexer.next();
        if(!next2){
            // ほんとはエラー
            return nullptr;
        }
        if(next2->is_semicolon()){
            pos += next2->pos;
            ret = std::make_unique<sentence::Declaration>(std::move(identifier).value(), std::move(type), nullptr);
        }else if(next2->is_equal()){
            auto rhs = parse_expression(lexer);
            if(!rhs){
                // エラー
                return nullptr;
            }
            auto next3 = lexer.next();
            if(!next3){
                // エラー
            }
            if(!next3->is_semicolon()){
                // エラー
            }
            pos += next3->pos;
            ret = std::make_unique<sentence::Declaration>(std::move(identifier).value(), std::move(type), std::move(rhs));
        }else{
            // ほんとはエラー
            return nullptr;
        }
    }else{
        // ほんとはエラー
        return nullptr;
    }
    ret->pos = pos;
    return ret;
}
