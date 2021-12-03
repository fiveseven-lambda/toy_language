#include "sentence.hpp"

#include <sstream>

namespace sentence {
    Sentence::~Sentence() = default;
    
    Expression::Expression(std::unique_ptr<expression::Expression> expression): expression(std::move(expression)) {}
    Declaration::Declaration(std::string name, std::shared_ptr<type::Type> type, std::unique_ptr<expression::Expression> expression):
        name(std::move(name)),
        type(std::move(type)),
        expression(std::move(expression)) {}

    void Expression::translate(context::Context &context, std::unique_ptr<llvm::Module> &module, std::map<std::string, std::pair<std::shared_ptr<type::Type>, std::size_t>> &global_variables){
        if(expression){
            std::map<std::string, std::pair<std::shared_ptr<type::Type>, llvm::Value *>> variables;
            for(auto &[name, variable]: global_variables){
                std::stringstream number;
                number << "v" << variable.second;
                llvm::Value *ptr = new llvm::GlobalVariable(
                    *module,
                    variable.first->llvm_type(context),
                    false,
                    llvm::GlobalValue::ExternalLinkage,
                    nullptr,
                    number.str()
                );
                variables.emplace(name, std::make_pair(variable.first, ptr));
            }
            auto [type, value] = expression->rvalue(context, variables);
            context.builder.CreateRet(value);
            // context.builder.CreateRetVoid();
        }
    }
    void Declaration::translate(context::Context &context, std::unique_ptr<llvm::Module> &module, std::map<std::string, std::pair<std::shared_ptr<type::Type>, std::size_t>> &global_variables){
        std::shared_ptr<type::Type> rhs_type;
        llvm::Value *rhs_value;
        if(expression){
            std::map<std::string, std::pair<std::shared_ptr<type::Type>, llvm::Value *>> variables;
            for(auto &[key, variable]: global_variables){
                std::stringstream number;
                number << "v" << variable.second;
                llvm::Value *ptr = new llvm::GlobalVariable(
                    *module,
                    variable.first->llvm_type(context),
                    false,
                    llvm::GlobalValue::ExternalLinkage,
                    nullptr,
                    number.str()
                );
                variables.emplace(key, std::make_pair(variable.first, ptr));
            }
            std::tie(rhs_type, rhs_value) = expression->rvalue(context, variables);
        }
        if(type){
            if(expression){
                rhs_value = type->convert_from(rhs_type, rhs_value, context);
            }
        }else{
            type = rhs_type;
        }
        // ここで番号付けに size を取るのは間違っている（同名の変数が複数個宣言された場合）
        std::size_t next_number = global_variables.size();
        std::stringstream next_number_string;
        next_number_string << "v" << next_number;
        auto variable = new llvm::GlobalVariable(*module.get(), type->llvm_type(context), false, llvm::GlobalValue::CommonLinkage, type->default_value(context), next_number_string.str());
        if(expression){
            context.builder.CreateStore(rhs_value, variable);
        }
        global_variables[name] = std::make_pair(type, next_number);
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
