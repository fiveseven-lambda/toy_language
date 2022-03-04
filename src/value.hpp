/**
 * @file value.hpp
 */
#ifndef VALUE_HPP
#define VALUE_HPP

#include <memory>
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Constant.h"

/**
 * @brief `llvm::Value *` と型情報の組を扱う
 *
 * @todo 関数型を追加する
 */
namespace value {
    /**
     * @brief 型
     */
    class Type {
    public:
        virtual ~Type();
        virtual llvm::Type *llvm_type(llvm::LLVMContext &) = 0;
        virtual llvm::Constant *default_value(llvm::LLVMContext &) = 0;
    };

    /**
     * @brief `llvm::Value *` と `Type` の組
     */
    struct Value {
        std::shared_ptr<value::Type> type;
        /**
         * @brief ポインタ
         */
        llvm::Value *llvm_value;
    public:
        Value(std::shared_ptr<value::Type>, llvm::Value *);
        Value();
    };

    template<class T>
    static Value make(llvm::Value *llvm_value){
        return Value(std::make_shared<T>(), llvm_value);
    }

    class Integer : public Type {
        llvm::Type *llvm_type(llvm::LLVMContext &) override;
        llvm::Constant *default_value(llvm::LLVMContext &) override;
    };

    class Boolean : public Type {
        llvm::Type *llvm_type(llvm::LLVMContext &) override;
        llvm::Constant *default_value(llvm::LLVMContext &) override;
    };
}

#endif
