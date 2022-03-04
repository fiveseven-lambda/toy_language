/**
 * @file sentence.cpp
 */
#include "sentence.hpp"

#include <sstream>

namespace sentence {
    Sentence::~Sentence() = default;
    //! コンストラクタ
    Expression::Expression(std::unique_ptr<expression::Expression> expression):
        expression(std::move(expression)) {}
    /**
     * @brief コンストラクタ
     * @param name 宣言された変数名
     * @param type 型
     * @param expression 初期化の式
     */
    Declaration::Declaration(
        std::string name,
        std::unique_ptr<type::Type> type,
        std::unique_ptr<expression::Expression> expression
    ):
        name(std::move(name)),
        type(std::move(type)),
        expression(std::move(expression)) {}
    /**
     * @brief コンストラクタ
     * @param sentences 中身
     */
    Block::Block(std::vector<std::unique_ptr<Sentence>> sentences):
        sentences(std::move(sentences)) {}
    /**
     * @brief コンストラクタ
     * @param condition 条件
     * @param if_clause if 節
     * @param else_clause else 節（空なら nullptr）
     */
    If::If(
        std::unique_ptr<expression::Expression> condition,
        std::unique_ptr<Sentence> if_clause,
        std::unique_ptr<Sentence> else_clause
    ):
        condition(std::move(condition)),
        if_clause(std::move(if_clause)),
        else_clause(std::move(else_clause)) {}
    /**
     * @brief コンストラクタ
     * @param condition 条件
     * @param sentence 中身
     */
    While::While(
        std::unique_ptr<expression::Expression> condition,
        std::unique_ptr<Sentence> sentence
    ):
        condition(std::move(condition)),
        sentence(std::move(sentence)) {}

    static std::string global_variable_name(unsigned module_number){
        std::stringstream ret;
        ret << "g" << module_number;
        return ret.str();
    }

    static llvm::Function *create_function(unsigned module_number, llvm::LLVMContext &context, llvm::Module &module){
        llvm::Type *void_type = llvm::Type::getVoidTy(context);
        llvm::FunctionType *function_type = llvm::FunctionType::get(void_type, {}, false);
        std::stringstream function_name;
        function_name << "f" << module_number;
        return llvm::Function::Create(function_type, llvm::Function::ExternalLinkage, function_name.str(), module);
    }

    llvm::orc::ThreadSafeModule Sentence::compile(Environment &environment){
        auto &module = environment.next_module();
        llvm::LLVMContext &context_ref = *environment.context.getContext();
        llvm::Function *function = create_function(environment.get_module_number(), context_ref, module);
        llvm::BasicBlock *basic_block = llvm::BasicBlock::Create(context_ref, "", function);
        environment.builder.SetInsertPoint(basic_block);
        std::unordered_map<std::string, value::Value> local_variables;
        compile_inner(environment, local_variables);
        return llvm::orc::ThreadSafeModule(environment.take_module(), environment.context);
    }
    void Expression::compile_inner(Environment &environment, std::unordered_map<std::string, value::Value> &local_variables){
        expression->compile(environment, local_variables);
    }
    void Declaration::compile_inner(Environment &environment, std::unordered_map<std::string, value::Value> &local_variables){
        if(expression){
            value::Value init_value = expression->compile(environment, local_variables);
            auto variable = new llvm::GlobalVariable(
                environment.get_module(),
                init_value.type->llvm_type(*environment.context.getContext()),
                false,
                llvm::GlobalValue::CommonLinkage,
                init_value.type->default_value(*environment.context.getContext()),
                global_variable_name(environment.get_module_number())
            );
            environment.builder.CreateStore(init_value.llvm_value, variable);
        }
    }
    void Block::compile_inner(Environment &, std::unordered_map<std::string, value::Value> &){}
    void If::compile_inner(Environment &, std::unordered_map<std::string, value::Value> &){}
    void While::compile_inner(Environment &, std::unordered_map<std::string, value::Value> &){}

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
    void Block::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": Block (" << sentences.size() << " sentences)" << std::endl;
        for(auto &sentence : sentences){
            sentence->debug_print(depth + 1);
        }
    }
    void If::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": If" << std::endl;
        condition->debug_print(depth + 1);
        if_clause->debug_print(depth + 1);
        if(else_clause) else_clause->debug_print(depth + 1);
    }
    void While::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": While" << std::endl;
        condition->debug_print(depth + 1);
        sentence->debug_print(depth + 1);
    }
}
