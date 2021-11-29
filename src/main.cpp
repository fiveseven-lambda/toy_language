#include "pos.hpp"
#include "error.hpp"
#include "token.hpp"
#include "lexer.hpp"
#include "syntax.hpp"
#include "parser.hpp"

int main(){
    lexer::Lexer lexer;
    try{
        while(true){
            std::unique_ptr<syntax::Expression> expression = parser::parse_expression(lexer);
            if(!expression) break;
            expression->debug_print();
        }
    }catch(std::unique_ptr<error::Error> &error){
        error->eprint(lexer.get_log());
    }
}
