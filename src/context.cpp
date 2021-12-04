#include "context.hpp"

#include <memory>

namespace context {
    Context::Context():
        context(std::make_unique<llvm::LLVMContext>()),
        builder(*context.getContext()),
        boolean_type(llvm::Type::getInt1Ty(*context.getContext())),
        integer_type(llvm::Type::getInt32Ty(*context.getContext())),
        void_type(llvm::Type::getVoidTy(*context.getContext())),
        function_type(llvm::FunctionType::get(void_type, false)) {}
}


