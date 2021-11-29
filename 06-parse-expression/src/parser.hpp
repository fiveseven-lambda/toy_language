#ifndef PARSER_HPP
#define PARSER_HPP

#include <memory>
#include <vector>

#include "syntax.hpp"
#include "lexer.hpp"

namespace parser {
    std::unique_ptr<syntax::Expression> parse_expression(lexer::Lexer &);
}

#endif
