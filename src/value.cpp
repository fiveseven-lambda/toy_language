#include "value.hpp"

#include "llvm/IR/DerivedTypes.h"

namespace value {
    Type::~Type() = default;
    llvm::Type *Integer::llvm_type(llvm::LLVMContext &context){
        return llvm::Type::getInt32Ty(context);
    }
    llvm::Type *Boolean::llvm_type(llvm::LLVMContext &context){
        return llvm::Type::getInt1Ty(context);
    }
}
