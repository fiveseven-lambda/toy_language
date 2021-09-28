#include "parser.hpp"
#include "error.hpp"

#include <iostream>

static std::unique_ptr<syntax::Expression> parse_factor(Lexer &), parse_binary_operator(Lexer &, int), parse_expression(Lexer &);
static std::unique_ptr<syntax::Type> parse_type(Lexer &);
static std::vector<std::unique_ptr<syntax::Expression>> parse_arguments(Lexer &);

std::unique_ptr<syntax::Expression> parse_factor(Lexer &lexer){
    std::unique_ptr<syntax::Expression> factor;
    {
        auto &token = lexer.peek();
        if(!token) return nullptr;
        if(auto name = token->identifier()){
            lexer.consume();
            factor = std::make_unique<syntax::Identifier>(name.value());
        }else if(auto value = token->positive_integer()){
            lexer.consume();
            factor = std::make_unique<syntax::Integer>(value.value());
        }else if(auto prefix = token->prefix()){
            lexer.consume();
            if(
                prefix.value() == syntax::UnaryOperator::Minus
                && (value = lexer.peek()->negative_integer())
            ){
                factor = std::make_unique<syntax::Integer>(value.value());
            }else{
                factor = std::make_unique<syntax::Unary>(prefix.value(), parse_factor(lexer));
            }
        }else if(token->is_opening_parenthesis()){
            lexer.consume();
            factor = parse_expression(lexer);
            if(auto close = lexer.next(); !(close && close->is_closing_parenthesis())){
                throw error::make<error::NoClosingParenthesis>();
            }
        }else{
            return nullptr;
        }
    }
    for(;;){
        auto &token = lexer.peek();
        if(!token) return factor;
        if(token->is_opening_parenthesis()){
            lexer.consume();
            auto arguments = parse_arguments(lexer);
            if(auto close = lexer.next(); !(close && close->is_closing_parenthesis())){
                throw error::make<error::NoClosingParenthesis>();
            }
            factor = std::make_unique<syntax::Invocation>(std::move(factor), std::move(arguments));
        }else{
            return factor;
        }
    }
}

std::unique_ptr<syntax::Expression> parse_binary_operator(Lexer &lexer, int precedence){
    if(precedence == syntax::MaxPrecedence) return parse_factor(lexer);
    auto left = parse_binary_operator(lexer, precedence + 1);
    for(;;){
        if(auto &token = lexer.peek()){
            if(auto binary_operator = token->binary_operator()){
                if(syntax::precedence(binary_operator.value()) == precedence){
                    lexer.consume();
                    auto right = parse_binary_operator(lexer, precedence + 1);
                    left = std::make_unique<syntax::Binary>(binary_operator.value(), std::move(left), std::move(right));
                    continue;
                }
            }
        }
        return left;
    }
}

std::unique_ptr<syntax::Expression> parse_expression(Lexer &lexer){
    return parse_binary_operator(lexer, 0);
}

std::vector<std::unique_ptr<syntax::Expression>> parse_arguments(Lexer &lexer){
    std::vector<std::unique_ptr<syntax::Expression>> ret;
    for(;;){
        auto argument = parse_expression(lexer);
        auto &token = lexer.peek();
        if(token->is_comma()){
            lexer.consume();
            ret.push_back(std::move(argument));
        }else{
            if(argument){
                ret.push_back(std::move(argument));
            }
            return ret;
        }
    }
}

std::unique_ptr<syntax::Sentence> parse_sentence(Lexer &lexer){
    auto expression = parse_expression(lexer);
    auto &token = lexer.peek();
    if(!token){
        return nullptr;
    }else if(token->is_semicolon()){
        lexer.consume();
        return std::make_unique<syntax::ExpressionSentence>(std::move(expression));
    }else if(token->is_equal()){
        lexer.consume();
        auto right = parse_expression(lexer);
        if(auto semicolon = lexer.next(); !(semicolon && semicolon->is_semicolon())){
            throw error::make<error::SyntaxError>();
        }
        return std::make_unique<syntax::Substitution>(std::move(expression), std::move(right));
    }else if(token->is_colon()){
        auto name = expression->into_identifier();
        if(!name){
            throw error::make<error::SyntaxError>();
        }
        lexer.consume();
        auto type = parse_type(lexer);
        if(auto equal = lexer.next(); !(equal && equal->is_equal())){
            throw error::make<error::SyntaxError>();
        }
        auto right = parse_expression(lexer);
        if(auto semicolon = lexer.next(); !(semicolon && semicolon->is_semicolon())){
            throw error::make<error::SyntaxError>();
        }
        return std::make_unique<syntax::Declaration>(std::move(name.value()), std::move(type), std::move(right));
    }else{
        throw error::make<error::SyntaxError>();
    }
}

std::unique_ptr<syntax::Type> parse_type(Lexer &lexer){
    auto &token = lexer.peek();
    if(!token){
        return nullptr;
    }else if(auto type = token->type()){
        lexer.consume();
        return type;
    }else{
        return nullptr;
    }
}
