#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/ConstantFolder.h"
#include "llvm/ExecutionEngine/Orc/ThreadSafeModule.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"

namespace context {
    struct Context {
        llvm::orc::ThreadSafeContext context;
        llvm::IRBuilder<llvm::ConstantFolder, llvm::IRBuilderDefaultInserter> builder;
        llvm::Type *boolean_type, *integer_type;
        llvm::FunctionType *function_type;
        Context();
    };
}

#endif
