/**
 * @file main.cpp
 */

#include "parser.hpp"
#include "error.hpp"
#include "environment.hpp"

int main(){
    Lexer lexer;
    Environment environment;
    try{
        while(true){
            auto sentence = parse_sentence(lexer);
            if(!sentence) break;
            sentence->debug_print();
            auto module = sentence->compile(environment);
            module.withModuleDo([](const llvm::Module &mod){ mod.print(llvm::errs(), nullptr); });
        }
    }catch(std::unique_ptr<error::Error> &error){
        error->eprint(lexer.get_log());
    }
}
