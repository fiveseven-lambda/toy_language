#include <iostream>
#include <string>

#include "pos.hpp"
#include "error.hpp"
#include "token.hpp"
#include "lexer.hpp"
#include "parser.hpp"

int main() {
    lexer::Lexer lexer;
    try {
        for(;;){
            auto expression = parser::parse_factor(lexer);
            if(!expression) break;
            std::cout << expression->pos << ": ";
            expression->debug_print();
        }
    }catch(std::unique_ptr<error::Error> &err){
        err->eprint(lexer.get_log());
    }
}
