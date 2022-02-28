/**
 * @file main.cpp
 */

#include "parser.hpp"
#include "error.hpp"

int main(){
    Lexer lexer;
    try{
        while(true){
            auto sentence = parse_sentence(lexer);
            if(!sentence) break;
            sentence->debug_print();
        }
    }catch(std::unique_ptr<error::Error> &error){
        error->eprint(lexer.get_log());
    }
}
