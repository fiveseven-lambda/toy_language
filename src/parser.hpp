#ifndef PARSER_HPP
#define PARSER_HPP

#include <memory>

#include "lexer.hpp"
#include "type.hpp"
#include "expression.hpp"
#include "sentence.hpp"

namespace parser {
    
    std::unique_ptr<expression::Expression> parse_expression(lexer::Lexer &);

    std::unique_ptr<type::Type> parse_type(lexer::Lexer &);

    
    std::unique_ptr<sentence::Sentence> parse_sentence(lexer::Lexer &);

}

#endif
