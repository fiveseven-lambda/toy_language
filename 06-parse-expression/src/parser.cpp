#include "parser.hpp"

namespace parser {
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
        }
        ret->pos = pos;
        return ret;
    }
}
