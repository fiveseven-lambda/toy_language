#ifndef LEXER_HPP
#define LEXER_HPP

#include <cstddef>
#include <string>
#include <queue>
#include <iostream>
#include <fstream>
#include <vector>

#include "pos.hpp"
#include "token.hpp"

class Lexer {
    class Inner {
        std::vector<pos::Pos> comment;
    public:
        void run(std::size_t, const std::string &, std::queue<std::unique_ptr<token::Token>> &);
        void deal_with_eof();
    } inner;

    std::istream &source;
    bool prompt;
    std::vector<std::string> log;
    std::queue<std::unique_ptr<token::Token>> tokens;
public:
    Lexer();
    Lexer(std::ifstream &);
    const std::vector<std::string> &get_log() const;
    std::unique_ptr<token::Token> next(), &peek();
};

#endif
