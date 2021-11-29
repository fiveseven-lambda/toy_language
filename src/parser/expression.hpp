#ifndef PARSER_EXPRESSION_HPP
#define PARSER_EXPRESSION_HPP

#include <memory>

#include "../syntax/expression.hpp"
#include "../lexer.hpp"

namespace parser {
    namespace expression {
        std::unique_ptr<syntax::expression::Expression> parse(lexer::Lexer &);
    }
}

#endif
