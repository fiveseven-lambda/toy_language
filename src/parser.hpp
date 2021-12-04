#ifndef PARSER_HPP
#define PARSER_HPP

#include <memory>

#include "lexer.hpp"
#include "sentence.hpp"

std::unique_ptr<sentence::Sentence> parse_sentence(Lexer &);

#endif