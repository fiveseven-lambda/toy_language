#include "parser.hpp"

#include "error.hpp"

static std::unique_ptr<expression::Expression> parse_factor(Lexer &lexer){
    auto &token_ref = lexer.peek();
    if(!token_ref) return nullptr;

    std::unique_ptr<expression::Expression> ret;
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
            pos += operand->pos;
            ret = std::make_unique<expression::UnaryOperation>(prefix.value(), std::move(operand));
        }
    }else if(token_ref->is_opening_parenthesis()){
        pos = std::move(lexer.next()->pos);
        ret = parse_expression(lexer);
        auto close = lexer.next();
        if(!close) throw error::make<error::NoClosingParenthesis>(std::move(pos));
        if(!close->is_closing_parenthesis()) throw error::make<error::UnexpectedTokenInParenthesis>(std::move(close->pos), std::move(pos));
        if(!ret) throw error::make<error::EmptyParenthesis>(std::move(pos));
        pos += close->pos;
    }else{
        throw error::make<error::UnexpectedTokenAtFactor>(std::move(lexer.next()->pos));
    }
    ret->pos = std::move(pos);
    return ret;
}

enum Precedence{
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

enum class Associativity{
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
        auto binary_operator = token->infix();
        if(binary_operator && precedence(binary_operator.value()) == current_precedence){
            auto &operator_pos = lexer.next()->pos;
            auto right = parse_binary_operator(lexer, current_precedence + left_to_right);
            if(!right) throw error::make<error::NoExpressionAfterOperator>(std::move(operator_pos));
            pos::Range pos = left->pos.clone();
            pos += right->pos;
            left = std::make_unique<expression::BinaryOperation>(binary_operator.value(), std::move(left), std::move(right));
            left->pos = std::move(pos);
            if(left_to_right) continue;
        }
        return left;
    }
}

std::unique_ptr<expression::Expression> parse_expression(Lexer &lexer){
    return parse_binary_operator(lexer, 0);
}
