#include <vector>

#include "expression.hpp"
#include "../error.hpp"

namespace parser {
    namespace expression {
        static std::unique_ptr<syntax::expression::Expression> parse_factor(lexer::Lexer &), parse_binary_operator(lexer::Lexer &, int precedence);
        static std::vector<std::unique_ptr<syntax::expression::Expression>> parse_arguments(lexer::Lexer &);

        std::unique_ptr<syntax::expression::Expression> parse_factor(lexer::Lexer &lexer){
            std::unique_ptr<syntax::expression::Expression> ret;
            {
                auto &token_ref = lexer.peek();
                if(!token_ref) return nullptr;

                pos::Range pos;
                if(auto name = token_ref->identifier()){
                    pos = lexer.next()->pos;
                    ret = std::make_unique<syntax::expression::Identifier>(std::move(name).value());
                }else if(auto value = token_ref->positive_integer()){
                    pos = lexer.next()->pos;
                    ret = std::make_unique<syntax::expression::Integer>(value.value());
                }else if(auto prefix = token_ref->prefix()){
                    pos = lexer.next()->pos;
                    if(prefix.value() == syntax::expression::UnaryOperator::Minus && (value = lexer.peek()->negative_integer())){
                        pos += lexer.next()->pos;
                        ret = std::make_unique<syntax::expression::Integer>(value.value());
                    }else{
                        auto operand = parse_factor(lexer);
                        pos += operand->pos;
                        ret = std::make_unique<syntax::expression::Unary>(prefix.value(), std::move(operand));
                    }
                }else if(token_ref->is_opening_parenthesis()){
                    pos = lexer.next()->pos;
                    ret = parse(lexer);
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
                    ret = std::make_unique<syntax::expression::Invocation>(std::move(ret), std::move(arguments));
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
        static Precedence precedence(syntax::expression::BinaryOperator binary_operator){
            switch(binary_operator){
                case syntax::expression::BinaryOperator::Mul:
                case syntax::expression::BinaryOperator::Div:
                case syntax::expression::BinaryOperator::Rem:
                    return MulDivRemPrecedence;
                case syntax::expression::BinaryOperator::Add:
                case syntax::expression::BinaryOperator::Sub:
                    return AddSubPrecedence;
                case syntax::expression::BinaryOperator::LeftShift:
                case syntax::expression::BinaryOperator::RightShift:
                    return ShiftPrecedence;
                case syntax::expression::BinaryOperator::BitAnd:
                    return BitAndPrecedence;
                case syntax::expression::BinaryOperator::BitXor:
                    return BitXorPrecedence;
                case syntax::expression::BinaryOperator::BitOr:
                    return BitOrPrecedence;
                case syntax::expression::BinaryOperator::Equal:
                case syntax::expression::BinaryOperator::NotEqual:
                case syntax::expression::BinaryOperator::Less:
                case syntax::expression::BinaryOperator::Greater:
                case syntax::expression::BinaryOperator::LessEqual:
                case syntax::expression::BinaryOperator::GreaterEqual:
                    return ComparisonPrecedence;
                case syntax::expression::BinaryOperator::LogicalAnd:
                    return LogicalAndPrecedence;
                case syntax::expression::BinaryOperator::LogicalOr:
                    return LogicalOrPrecedence;
                case syntax::expression::BinaryOperator::Assign:
                case syntax::expression::BinaryOperator::MulAssign:
                case syntax::expression::BinaryOperator::DivAssign:
                case syntax::expression::BinaryOperator::RemAssign:
                case syntax::expression::BinaryOperator::AddAssign:
                case syntax::expression::BinaryOperator::SubAssign:
                case syntax::expression::BinaryOperator::LeftShiftAssign:
                case syntax::expression::BinaryOperator::RightShiftAssign:
                case syntax::expression::BinaryOperator::BitAndAssign:
                case syntax::expression::BinaryOperator::BitXorAssign:
                case syntax::expression::BinaryOperator::BitOrAssign:
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
        std::unique_ptr<syntax::expression::Expression> parse_binary_operator(lexer::Lexer &lexer, int current_precedence){
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
                    left = std::make_unique<syntax::expression::Binary>(binary_operator.value(), std::move(left), std::move(right));
                    left->pos = pos;
                    if(left_to_right) continue;
                }
                return left;
            }
        }

        std::unique_ptr<syntax::expression::Expression> parse(lexer::Lexer &lexer){
            return parse_binary_operator(lexer, 0);
        }

        std::vector<std::unique_ptr<syntax::expression::Expression>> parse_arguments(lexer::Lexer &lexer){
            std::vector<std::unique_ptr<syntax::expression::Expression>> ret;
            while(true){
                auto argument = parse(lexer);
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
    }
}
