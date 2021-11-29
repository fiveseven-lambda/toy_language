#include "parser.hpp"
#include "error.hpp"

namespace parser {
    static std::unique_ptr<syntax::Expression> parse_factor(lexer::Lexer &), parse_binary_operator(lexer::Lexer &, int precedence);
    static std::vector<std::unique_ptr<syntax::Expression>> parse_arguments(lexer::Lexer &);

    std::unique_ptr<syntax::Expression> parse_factor(lexer::Lexer &lexer){
        auto &token_ref = lexer.peek();
        if(!token_ref) return nullptr;

        std::unique_ptr<syntax::Expression> ret;
        pos::Range pos;
        if(auto name = token_ref->identifier()){
            pos = lexer.next()->pos;
            ret = std::make_unique<syntax::Identifier>(std::move(name).value());
        }else if(auto value = token_ref->positive_integer()){
            pos = lexer.next()->pos;
            ret = std::make_unique<syntax::Integer>(value.value());
        }else if(auto prefix = token_ref->prefix()){
            pos = lexer.next()->pos;
            if(prefix.value() == syntax::UnaryOperator::Minus && (value = lexer.peek()->negative_integer())){
                pos += lexer.next()->pos;
                ret = std::make_unique<syntax::Integer>(value.value());
            }else{
                auto operand = parse_factor(lexer);
                pos += operand->pos;
                ret = std::make_unique<syntax::Unary>(prefix.value(), std::move(operand));
            }
        }else if(token_ref->is_opening_parenthesis()){
            pos = lexer.next()->pos;
            ret = parse_expression(lexer);
            if(!ret) throw error::make<error::EmptyParenthesis>(std::move(pos));
            auto close = lexer.next();
            if(close && close->is_closing_parenthesis()){
                pos += close->pos;
            }else{
                throw error::make<error::NoClosingParenthesis>(std::move(pos));
            }
        }else{
            return nullptr;
        }
        ret->pos = pos;
        return ret;
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
    static Precedence precedence(syntax::BinaryOperator binary_operator){
        switch(binary_operator){
            case syntax::BinaryOperator::Mul:
            case syntax::BinaryOperator::Div:
            case syntax::BinaryOperator::Rem:
                return MulDivRemPrecedence;
            case syntax::BinaryOperator::Add:
            case syntax::BinaryOperator::Sub:
                return AddSubPrecedence;
            case syntax::BinaryOperator::LeftShift:
            case syntax::BinaryOperator::RightShift:
                return ShiftPrecedence;
            case syntax::BinaryOperator::BitAnd:
                return BitAndPrecedence;
            case syntax::BinaryOperator::BitXor:
                return BitXorPrecedence;
            case syntax::BinaryOperator::BitOr:
                return BitOrPrecedence;
            case syntax::BinaryOperator::Equal:
            case syntax::BinaryOperator::NotEqual:
            case syntax::BinaryOperator::Less:
            case syntax::BinaryOperator::Greater:
            case syntax::BinaryOperator::LessEqual:
            case syntax::BinaryOperator::GreaterEqual:
                return ComparisonPrecedence;
            case syntax::BinaryOperator::LogicalAnd:
                return LogicalAndPrecedence;
            case syntax::BinaryOperator::LogicalOr:
                return LogicalOrPrecedence;
            case syntax::BinaryOperator::Assign:
            case syntax::BinaryOperator::MulAssign:
            case syntax::BinaryOperator::DivAssign:
            case syntax::BinaryOperator::RemAssign:
            case syntax::BinaryOperator::AddAssign:
            case syntax::BinaryOperator::SubAssign:
            case syntax::BinaryOperator::LeftShiftAssign:
            case syntax::BinaryOperator::RightShiftAssign:
            case syntax::BinaryOperator::BitAndAssign:
            case syntax::BinaryOperator::BitXorAssign:
            case syntax::BinaryOperator::BitOrAssign:
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
    std::unique_ptr<syntax::Expression> parse_binary_operator(lexer::Lexer &lexer, int current_precedence){
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
                left = std::make_unique<syntax::Binary>(binary_operator.value(), std::move(left), std::move(right));
                left->pos = pos;
                if(left_to_right) continue;
            }
            return left;
        }
    }

    std::unique_ptr<syntax::Expression> parse_expression(lexer::Lexer &lexer){
        return parse_binary_operator(lexer, 0);
    }
}