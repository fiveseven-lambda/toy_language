#include <iostream>

#include "lexer.hpp"

int main(){
    Lexer lexer;
    while(auto token = lexer.next()){
        token->print();
    }
}
