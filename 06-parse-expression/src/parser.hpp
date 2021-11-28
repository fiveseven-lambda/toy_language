#ifndef PARSER_HPP
#define PARSER_HPP

#include <memory>
#include <vector>

#include "syntax.hpp"
#include "lexer.hpp"

namespace parser {
    std::unique_ptr<syntax::Expression>
        parse_factor(lexer::Lexer &),
        parse_binary_operator(lexer::Lexer &),
        parse_expression(lexer::Lexer &);
    std::vector<std::unique_ptr<syntax::Expression>>
        parse_arguments(lexer::Lexer &);
}

#endif
