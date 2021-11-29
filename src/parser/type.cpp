#include "type.hpp"

namespace parser {
    namespace type {
        std::unique_ptr<syntax::type::Type> parse(lexer::Lexer &lexer){
            auto &token_ref = lexer.peek();
            if(!token_ref) return nullptr;
            auto type = token_ref->type_name();
            if(type) lexer.next();
            return type;
        }
    }
}
