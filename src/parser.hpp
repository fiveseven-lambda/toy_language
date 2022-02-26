/**
 * @file parser.hpp
 * @brief 構文解析を行う
 */
#ifndef PARSER_HPP
#define PARSER_HPP

#include <memory>

#include "lexer.hpp"
#include "expression.hpp"

std::unique_ptr<expression::Expression> parse_expression(Lexer &);

#endif
