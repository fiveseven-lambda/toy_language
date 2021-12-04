#include "pos.hpp"
#include "error.hpp"
#include "token.hpp"
#include "lexer.hpp"
#include "sentence.hpp"
#include "parser.hpp"
#include "environment.hpp"

int main(){
    Lexer lexer;
    environment::Environment environment;
    try{
        while(true){
            std::unique_ptr<sentence::Sentence> sentence = parse_sentence(lexer);
            if(!sentence) break;
            // sentence->debug_print();
            environment.run(sentence);
        }
    }catch(std::unique_ptr<error::Error> &error){
        error->eprint(lexer.get_log());
    }
}
