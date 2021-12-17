#include <iostream>
#include <memory>

#include "pos.hpp"
#include "error.hpp"
#include "token.hpp"
#include "lexer.hpp"

int main(){
    Lexer lexer;
    try{
        while(true){
            auto token = lexer.next();
            if(!token) break;
            std::cout << token->pos << " ";
            token->debug_print();
        }
    }catch(std::unique_ptr<error::Error> &error){
        error->eprint(lexer.get_log());
    }
}
