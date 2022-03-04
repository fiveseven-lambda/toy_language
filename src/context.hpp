/**
 * @file context.hpp
 */
#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <unordered_map>
#include <string>
#include <utility>

#include "value.hpp"

#include "llvm/ExecutionEngine/Orc/ThreadSafeModule.h"
#include "llvm/IR/IRBuilder.h"

/**
 * @brief コンパイルに必要な Context 等を関数間で取り回しやすくするためのクラス．
 * @todo `Context` に何をどこまで含めるか，層に分離する必要がないかは要検討．
 */
struct Context {
    llvm::orc::ThreadSafeContext context;
    llvm::IRBuilder<llvm::ConstantFolder, llvm::IRBuilderDefaultInserter> builder;
    std::unordered_map<std::string, std::pair<unsigned, std::shared_ptr<value::Type>>> global_variables;
    std::unique_ptr<llvm::Module> module;
    unsigned current_module_number;
public:
    llvm::Module &next_module(), &get_module();
    std::unique_ptr<llvm::Module> take_module();
    unsigned get_module_number();
    std::string function_name(), global_variable_name();
    std::string function_name(unsigned), global_variable_name(unsigned);
    Context();
};

#endif
