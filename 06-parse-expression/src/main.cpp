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
            std::unique_ptr<syntax::Expression> expression = parser::parse_factor(lexer);
            if(!expression) break;
            std::cout << expression->pos << std::endl;
            expression->debug_print();
        }
    }catch(std::unique_ptr<error::Error> &error){
        error->eprint(lexer.get_log());
    }
}
