/**
 * @file lexer.hpp
 * @brief 字句解析を行う
 */
#ifndef LEXER_HPP
#define LEXER_HPP

#include <cstddef>
#include <string>
#include <queue>
#include <iostream>
#include <fstream>
#include <vector>
#include <vector>

#include "token.hpp"

/**
 * @brief 字句解析を行うクラス
 *
 * 入力を読み取って，字句解析を行う．
 */
class Lexer {
    std::istream &source;
    bool prompt;
    class Inner {
        std::vector<pos::Pos> comment;
    public:
        void run(
            std::size_t,
            const std::string &,
            std::queue<std::unique_ptr<token::Token>> &
        );
        void deal_with_eof();
    } inner;
    std::vector<std::string> log;
    std::queue<std::unique_ptr<token::Token>> tokens;
public:
    Lexer();
    Lexer(std::ifstream &);
    const std::vector<std::string> &get_log() const;
    std::unique_ptr<token::Token> next(), &peek();
};

#endif
