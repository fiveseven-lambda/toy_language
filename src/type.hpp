#ifndef TYPE_HPP
#define TYPE_HPP

#include <memory>

#include "pos.hpp"

namespace type {
    class Type {
    public:
        pos::Range pos;
        virtual ~Type();
        virtual void debug_print(int = 0) const = 0;
    };

    class Integer : public Type {
        void debug_print(int) const override;
    };

    class Boolean : public Type {
        void debug_print(int) const override;
    };
}

#endif
