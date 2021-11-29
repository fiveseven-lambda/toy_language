#include "pos.hpp"
#include "error.hpp"
#include "token.hpp"
#include "lexer.hpp"
#include "sentence.hpp"
#include "parser.hpp"

int main(){
    lexer::Lexer lexer;
    try{
        while(true){
            std::unique_ptr<sentence::Sentence> sentence = parser::parse_sentence(lexer);
            if(!sentence) break;
            sentence->debug_print();
        }
    }catch(std::unique_ptr<error::Error> &error){
        error->eprint(lexer.get_log());
    }
}
