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
            factor = std::make_unique<syntax::Unary>(parse_factor(lexer), prefix.value());
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

std::unique_ptr<syntax::Expression> parse_expression(Lexer &lexer){
    return parse_factor(lexer);
}
