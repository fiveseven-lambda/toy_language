/**
 * @file context.cpp
 */
#include "context.hpp"

#include <sstream>

//! コンストラクタ
Context::Context():
    context(std::make_unique<llvm::LLVMContext>()),
    builder(*context.getContext()),
    current_module_number(0) {}

static std::string module_name(unsigned module_number){
    std::stringstream ret;
    ret << "m" << module_number;
    return ret.str();
}

llvm::Module &Context::next_module(){
    current_module_number++;
    module = std::make_unique<llvm::Module>(module_name(current_module_number), *context.getContext());
    return *module;
}

llvm::Module &Context::get_module(){ return *module; }
std::unique_ptr<llvm::Module> Context::take_module(){ return std::move(module); }

unsigned Context::get_module_number(){
    return current_module_number;
}

std::string Context::function_name(){
    return function_name(current_module_number);
}
std::string Context::function_name(unsigned module_number){
    std::stringstream ret;
    ret << "f" << module_number;
    return ret.str();
}

std::string Context::global_variable_name(){
    return global_variable_name(current_module_number);
}
std::string Context::global_variable_name(unsigned module_number){
    std::stringstream ret;
    ret << "g" << module_number;
    return ret.str();
}
