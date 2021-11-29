#include "pos.hpp"
#include "error.hpp"
#include "token.hpp"
#include "lexer.hpp"
#include "syntax/sentence.hpp"
#include "parser/sentence.hpp"

int main(){
    lexer::Lexer lexer;
    try{
        while(true){
            std::unique_ptr<syntax::sentence::Sentence> sentence = parser::sentence::parse(lexer);
            if(!sentence) break;
            sentence->debug_print();
        }
    }catch(std::unique_ptr<error::Error> &error){
        error->eprint(lexer.get_log());
    }
}
