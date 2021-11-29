#ifndef PARSER_TYPE_HPP
#define PARSER_TYPE_HPP

#include <memory>
#include "../syntax/type.hpp"
#include "../lexer.hpp"

namespace parser {
    namespace type {
        std::unique_ptr<syntax::type::Type> parse(lexer::Lexer &);
    }
}

#endif
