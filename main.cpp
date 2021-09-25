#include <iostream>

#include "lexer.hpp"
#include "parser.hpp"
#include "error.hpp"

int main(){
    Lexer lexer;
    try{
        for(;;){
            auto sentence = parse_sentence(lexer);
            if(!sentence){
                break;
            }
            sentence->print();
        }
    }catch(std::unique_ptr<error::Error> &e){
        e->print();
    }
}
