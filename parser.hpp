#ifndef PARSER_HPP
#define PARSER_HPP

#include <memory>
#include "lexer.hpp"
#include "syntax.hpp"

std::unique_ptr<syntax::Expression> parse_expression(Lexer &);

#endif
