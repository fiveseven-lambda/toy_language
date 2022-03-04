/**
 * @file value.cpp
 */
#include "value.hpp"

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Constants.h"

namespace value {
    Type::~Type() = default;
    llvm::Type *Integer::llvm_type(llvm::LLVMContext &context){
        return llvm::Type::getInt32Ty(context);
    }
    llvm::Type *Boolean::llvm_type(llvm::LLVMContext &context){
        return llvm::Type::getInt1Ty(context);
    }
    llvm::Constant *Integer::default_value(llvm::LLVMContext &context){
        return llvm::ConstantInt::getSigned(llvm_type(context), 0);
    }
    llvm::Constant *Boolean::default_value(llvm::LLVMContext &context){
        return llvm::ConstantInt::getFalse(context);
    }
    Value::Value(): llvm_value(nullptr) {}
    Value::Value(std::shared_ptr<value::Type> type, llvm::Value *llvm_value): type(std::move(type)), llvm_value(llvm_value) {}
}
