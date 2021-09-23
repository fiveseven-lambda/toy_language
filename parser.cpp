#include "parser.hpp"

#include <iostream>

static std::unique_ptr<syntax::Expression> parse_factor(Lexer &lexer){
    std::unique_ptr<syntax::Expression> factor;
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
            std::cerr << "syntax error" << std::endl;
        }
    }else{
        return nullptr;
    }
    return factor;
}

static std::unique_ptr<syntax::Expression> parse_binary_operator(Lexer &lexer, int precedence){
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
