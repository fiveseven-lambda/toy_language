#ifndef TYPE_HPP
#define TYPE_HPP

#include <memory>

#include "pos.hpp"
#include "context.hpp"

#include "llvm/IR/Type.h"

namespace type {
    class Type {
    public:
        pos::Range pos;
        virtual ~Type();
        virtual llvm::Type *llvm_type(context::Context &) = 0;
        virtual llvm::Constant *default_value(context::Context &) = 0;
        virtual llvm::Value
            *convert_from(std::shared_ptr<Type>, llvm::Value *, context::Context &) = 0,
            *convert_into_integer(llvm::Value *, context::Context &) = 0,
            *convert_into_boolean(llvm::Value *, context::Context &) = 0;
        virtual void debug_print(int = 0) const = 0;
    };

    class Integer : public Type {
    public:
        llvm::Type *llvm_type(context::Context &) override;
        llvm::Constant *default_value(context::Context &) override;
        llvm::Value
            *convert_from(std::shared_ptr<Type>, llvm::Value *, context::Context &) override,
            *convert_into_integer(llvm::Value *, context::Context &) override,
            *convert_into_boolean(llvm::Value *, context::Context &) override;
        void debug_print(int) const override;
    };

    class Boolean : public Type {
    public:
        llvm::Type *llvm_type(context::Context &) override;
        llvm::Constant *default_value(context::Context &) override;
        llvm::Value
            *convert_from(std::shared_ptr<Type>, llvm::Value *, context::Context &) override,
            *convert_into_integer(llvm::Value *, context::Context &) override,
            *convert_into_boolean(llvm::Value *, context::Context &) override;
        void debug_print(int) const override;
    };
}

#endif
