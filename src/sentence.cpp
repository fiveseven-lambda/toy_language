#include "sentence.hpp"

namespace sentence {
    Sentence::~Sentence() = default;
    Expression::Expression(std::unique_ptr<expression::Expression> expression):
        expression(std::move(expression)) {}
    Declaration::Declaration(
        std::string name,
        std::unique_ptr<type::Type> type,
        std::unique_ptr<expression::Expression> expression
    ):
        name(std::move(name)),
        type(std::move(type)),
        expression(std::move(expression)) {}

    static constexpr std::string_view INDENT = "    ";
    void Expression::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        if(expression){
            std::cout << pos << ": Expression" << std::endl;
            expression->debug_print(depth + 1);
        }else{
            std::cout << pos << ": Expression (empty)" << std::endl;
        }
    }
    void Declaration::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": Declaration(" << name << ")" << std::endl;
        if(type) type->debug_print(depth + 1);
        if(expression) expression->debug_print(depth + 1);
    }
}
