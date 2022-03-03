/**
 * @brief
 */
#ifndef VALUE_HPP
#define VALUE_HPP

#include <memory>
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/LLVMContext.h"

namespace value {
    class Type {
    public:
        virtual ~Type();
        virtual llvm::Type *llvm_type(llvm::LLVMContext &) = 0;
    };

    class Integer : public Type {
        llvm::Type *llvm_type(llvm::LLVMContext &) override;
    };

    class Boolean : public Type {
        llvm::Type *llvm_type(llvm::LLVMContext &) override;
    };

    class Value {
        std::shared_ptr<value::Type> type;
        llvm::Value *llvm_value;
    };
}

#endif
