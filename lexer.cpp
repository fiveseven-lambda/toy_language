#include "lexer.hpp"

#include <iostream>

Lexer::Lexer():
    input(std::cin, true) {}
Lexer::Lexer(std::ifstream &file):
    input(file, false) {}

std::unique_ptr<token::Token> &Lexer::peek(){
    if(!peeked){
        while(std::isspace(input.peek())) input.get();
        int first = input.get();
        if(first == EOF){
            peeked = nullptr;
        }else if(std::isalpha(first)){
            std::string ret(1, std::string::traits_type::to_char_type(first));
            while(std::isalnum(input.peek())) ret.push_back(std::string::traits_type::to_char_type(input.get()));
            peeked = std::make_unique<token::Identifier>(std::move(ret));
        }else if(first == '+'){
            peeked = std::make_unique<token::Plus>();
        }else if(first == '-'){
            peeked = std::make_unique<token::Hyphen>();
        }else if(first == '*'){
            peeked = std::make_unique<token::Asterisk>();
        }else if(first == '('){
            peeked = std::make_unique<token::OpeningParenthesis>();
        }else if(first == ')'){
            peeked = std::make_unique<token::ClosingParenthesis>();
        }else if(first == '/'){
            switch(input.peek()){
                case '*':
                    input.get();
                    for(std::size_t depth = 1; depth > 0;){
                        auto c = input.get();
                        if(c == '*' && input.peek() == '/'){
                            input.get();
                            --depth;
                        }else if(c == '/' && input.peek() == '*'){
                            input.get();
                            ++depth;
                        }
                    }
                    return peek();
                case '/':
                    input.get();
                    while(input.get() != '\n');
                    return peek();
                default:
                    peeked = std::make_unique<token::Slash>();
            }
        }else if(std::isdigit(first)){ 
            std::string ret(1, std::string::traits_type::to_char_type(first));
            while(std::isdigit(input.peek())) ret.push_back(std::string::traits_type::to_char_type(input.get()));
            peeked = std::make_unique<token::Integer>(std::move(ret));
        }
    }
    return peeked.value();
}

void Lexer::consume(){
    peeked = std::nullopt;
}

std::unique_ptr<token::Token> Lexer::next(){
    peek();
    auto ret = std::move(peeked).value();
    peeked = std::nullopt;
    return ret;
}
