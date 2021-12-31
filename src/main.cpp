#include <iostream>
#include <memory>

#include "pos.hpp"
#include "error.hpp"
#include "token.hpp"
#include "lexer.hpp"
#include "parser.hpp"

int main(){
    Lexer lexer;
    try{
        while(true){
            auto expression = parse_expression(lexer);
            if(!expression) break;
            expression->debug_print();
        }
    }catch(std::unique_ptr<error::Error> &error){
        error->eprint(lexer.get_log());
    }
}
