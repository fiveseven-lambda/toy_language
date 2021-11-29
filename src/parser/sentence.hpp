#ifndef PARSER_SENTENCE_HPP
#define PARSER_SENTENCE_HPP

#include <memory>

#include "../lexer.hpp"
#include "../syntax/sentence.hpp"

namespace parser {
    namespace sentence {
        std::unique_ptr<syntax::sentence::Sentence> parse(lexer::Lexer &);
    }
}

#endif
