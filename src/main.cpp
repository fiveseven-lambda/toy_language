#include <iostream>
#include <string>

#include "pos.hpp"
#include "error.hpp"
#include "token.hpp"
#include "lexer.hpp"

using namespace std::literals::string_literals;

int main() {
    lexer::Lexer lexer;
    try {
        for(;;){
            auto [pos, token] = lexer.next();
            if(!token) break;
            token->debug_print();
        }
    }catch(std::unique_ptr<error::Error> &err){
        err->eprint(lexer.get_log());
    }
}
