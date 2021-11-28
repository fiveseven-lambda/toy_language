#include "parser.hpp"

namespace parser {
    std::unique_ptr<syntax::Expression> parse_factor(lexer::Lexer &lexer){
        auto &token_ref = lexer.peek();
        if(!token_ref) return nullptr;
        std::unique_ptr<syntax::Expression> ret;
        if(auto name = token_ref->identifier()){
            pos::Range pos = lexer.next()->pos;
            ret = std::make_unique<syntax::Identifier>(std::move(name).value());
            ret->pos = pos;
        }else if(auto value = token_ref->positive_integer()){
            pos::Range pos = lexer.next()->pos;
            ret = std::make_unique<syntax::Integer>(value.value());
            ret->pos = pos;
        }
        return ret;
    }
}

