#include "sentence.hpp"

#include <sstream>

namespace sentence {
    Sentence::~Sentence() = default;
    
    Expression::Expression(std::unique_ptr<expression::Expression> expression): expression(std::move(expression)) {}
    Declaration::Declaration(std::string name, std::shared_ptr<type::Type> type, std::unique_ptr<expression::Expression> expression):
        name(std::move(name)),
        type(std::move(type)),
        expression(std::move(expression)) {}

    void Expression::translate(context::Context &context, std::unique_ptr<llvm::Module> &module, global_variables::GlobalVariables &global_variables){
        if(expression){
            auto variables = global_variables.variables(context, module);
            expression->rvalue(context, variables);
            context.builder.CreateRetVoid();
        }
    }
    void Declaration::translate(context::Context &context, std::unique_ptr<llvm::Module> &module, global_variables::GlobalVariables &global_variables){
        std::shared_ptr<type::Type> rhs_type;
        llvm::Value *rhs_value;
        if(expression){
            auto variables = global_variables.variables(context, module);
            std::tie(rhs_type, rhs_value) = expression->rvalue(context, variables);
        }
        if(type){
            if(expression){
                rhs_value = type->convert_from(rhs_type, rhs_value, context);
            }
        }else{
            type = rhs_type;
        }
        std::size_t number = global_variables.insert(name, type);
        std::stringstream llvm_name;
        llvm_name << "v" << number;
        auto variable = new llvm::GlobalVariable(*module.get(), type->llvm_type(context), false, llvm::GlobalValue::CommonLinkage, type->default_value(context), llvm_name.str());
        if(expression){
            context.builder.CreateStore(rhs_value, variable);
        }
    }

    static constexpr std::string_view INDENT = "    ";
    void Expression::debug_print(int depth) const {
        if(expression){
            for(int i = 0; i < depth; ++i) std::cout << INDENT;
            std::cout << pos << ": Expression Sentence" << std::endl;
            expression->debug_print(depth + 1);
        }else{
            for(int i = 0; i < depth; ++i) std::cout << INDENT;
            std::cout << pos << ": Expression Sentence (empty)" << std::endl;
        }
    }
    void Declaration::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": Declaration (" << name << ")" << std::endl;
        if(type){
            for(int i = 0; i < depth; ++i) std::cout << INDENT;
            std::cout << "type: " << std::endl;
            type->debug_print(depth + 1);
        }
        if(expression){
            for(int i = 0; i < depth; ++i) std::cout << INDENT;
            std::cout << "expression: " << std::endl;
            expression->debug_print(depth + 1);
        }
    }
}
