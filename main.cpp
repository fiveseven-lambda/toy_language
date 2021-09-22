#include <iostream>

#include "lexer.hpp"
#include "parser.hpp"

int main(){
    Lexer lexer;
    parse_expression(lexer)->print();
}
