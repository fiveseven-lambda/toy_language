#include <iostream>

#include "lexer.hpp"
#include "parser.hpp"
#include "error.hpp"

int main(){
    Lexer lexer;
    try{
        parse_expression(lexer)->print();
    }catch(std::unique_ptr<error::Error> &e){
        e->print();
    }
}
