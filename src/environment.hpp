/**
 * @file environment.hpp
 */
#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <unordered_map>
#include <string>
#include <utility>
#include <memory>

#include "type.hpp"

#include "llvm/ExecutionEngine/Orc/ThreadSafeModule.h"
#include "llvm/IR/IRBuilder.h"

struct Environment {
    llvm::orc::ThreadSafeContext context;
    llvm::IRBuilder<llvm::ConstantFolder, llvm::IRBuilderDefaultInserter> builder;
    std::unordered_map<std::string, std::pair<unsigned, std::shared_ptr<type::Type>>> global_variables;
    std::unique_ptr<llvm::Module> module;
    unsigned module_number;
public:
    llvm::Module &next_module(), &get_module();
    std::unique_ptr<llvm::Module> take_module();
    unsigned get_module_number();
    Environment();
};

#endif
