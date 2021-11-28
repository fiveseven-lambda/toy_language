#include "pos.hpp"
#include "error.hpp"
#include "token.hpp"
#include "lexer.hpp"

int main(){
    lexer::Lexer lexer;
    try{
        while(true){
            std::unique_ptr<token::Token> token = lexer.next();
            if(!token) break;
            std::cout << token->pos << '\t';
            token->debug_print();
        }
    }catch(std::unique_ptr<error::Error> &error){
        error->eprint(lexer.get_log());
    }
}
