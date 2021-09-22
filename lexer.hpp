#ifndef LEXER_HPP
#define LEXER_HPP

#include "input.hpp"
#include "token.hpp"
#include <fstream>
#include <memory>
#include <optional>

class Lexer {
    Input input;
    std::template optional<std::unique_ptr<token::Token>> peeked;
public:
    Lexer();
    Lexer(std::ifstream &);

    // 次のトークンを返す．
    // EOF なら nullptr
    std::unique_ptr<token::Token> &peek(), next();
    // peek されているトークンを捨てる．
    void consume();
};

#endif
