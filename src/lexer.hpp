#ifndef LEXER_HPP
#define LEXER_HPP

#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <memory>
#include <vector>

#include "token.hpp"

namespace lexer {
    class Inner {
        std::vector<pos::Pos> comment;
    public:
        void run(std::size_t, const std::string &, std::queue<token::TokenWithPos> &);
        void deal_with_eof();
    };

    class Lexer {
        std::istream &source;
        bool prompt;
        Inner inner;
        std::vector<std::string> log;
        std::queue<token::TokenWithPos> tokens;
    public:
        Lexer();
        Lexer(std::ifstream &);
        const std::vector<std::string> &get_log() const;
        token::TokenWithPos next(), &peek();
    };
}

#endif
