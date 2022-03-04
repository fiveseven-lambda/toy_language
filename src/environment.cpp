/**
 * @file environment.cpp
 */
#include "environment.hpp"

#include <sstream>

Environment::Environment():
    context(std::make_unique<llvm::LLVMContext>()),
    builder(*context.getContext()),
    module_number(0) {}

static std::string module_name(unsigned module_number){
    std::stringstream ret;
    ret << "m" << module_number;
    return ret.str();
}

llvm::Module &Environment::next_module(){
    module_number++;
    module = std::make_unique<llvm::Module>(module_name(module_number), *context.getContext());
    return *module;
}

llvm::Module &Environment::get_module(){ return *module; }
std::unique_ptr<llvm::Module> Environment::take_module(){ return std::move(module); }

unsigned Environment::get_module_number(){
    return module_number;
}
