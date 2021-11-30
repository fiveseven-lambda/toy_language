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
        virtual void debug_print(int = 0) const = 0;
    };

    class Integer : public Type {
        llvm::Type *llvm_type(context::Context &) override;
        void debug_print(int) const override;
    };

    class Boolean : public Type {
        llvm::Type *llvm_type(context::Context &) override;
        void debug_print(int) const override;
    };
}

#endif
