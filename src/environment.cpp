/**
 * @file environment.cpp
 */
#include "environment.hpp"

#include <sstream>

Environment::Environment():
    context(std::make_unique<llvm::LLVMContext>()),
    module_number(0)
    {}

std::string Environment::module_name(){
    std::stringstream ss;
    ss << "m" << module_number;
    return ss.str();
}

llvm::orc::ThreadSafeModule Environment::compile(std::unique_ptr<sentence::Sentence>){
    auto module = std::make_unique<llvm::Module>(module_name(), *context.getContext());

    ++module_number;
    return llvm::orc::ThreadSafeModule(std::move(module), context);
}
