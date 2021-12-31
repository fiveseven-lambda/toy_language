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
            ret = std::make_unique<expression::Unary>(prefix.value(), std::move(operand));
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

std::unique_ptr<expression::Expression> parse_expression(Lexer &lexer){
    return parse_factor(lexer);
}
