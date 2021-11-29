#ifndef SYNTAX_TYPE_HPP
#define SYNTAX_TYPE_HPP

#include "../pos.hpp"

namespace syntax {
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
}

#endif
