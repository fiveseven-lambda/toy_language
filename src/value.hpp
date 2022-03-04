/**
 * @brief
 */
#ifndef VALUE_HPP
#define VALUE_HPP

#include <memory>
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Constant.h"

namespace value {
    class Type {
    public:
        virtual ~Type();
        virtual llvm::Type *llvm_type(llvm::LLVMContext &) = 0;
        virtual llvm::Constant *default_value(llvm::LLVMContext &) = 0;
    };

    class Integer : public Type {
        llvm::Type *llvm_type(llvm::LLVMContext &) override;
        llvm::Constant *default_value(llvm::LLVMContext &) override;
    };

    class Boolean : public Type {
        llvm::Type *llvm_type(llvm::LLVMContext &) override;
        llvm::Constant *default_value(llvm::LLVMContext &) override;
    };

    struct Value {
        std::shared_ptr<value::Type> type;
        /**
         * @brief ポインタ
         */
        llvm::Value *llvm_value;
    public:
        Value(std::shared_ptr<value::Type>, llvm::Value *);
    };

    template<class T>
    static Value make(llvm::Value *llvm_value){
        return Value(std::make_shared<T>(), llvm_value);
    }
}

#endif
