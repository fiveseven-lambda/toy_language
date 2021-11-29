#include "type.hpp"

namespace syntax {
    namespace type {
        Type::~Type() = default;

        static constexpr std::string_view INDENT = "    ";
        void Integer::debug_print(int depth) const {
            for(int i = 0; i < depth; ++i) std::cout << INDENT;
            std::cout << pos << ": integer" << std::endl;
        }
        void Boolean::debug_print(int depth) const {
            for(int i = 0; i < depth; ++i) std::cout << INDENT;
            std::cout << pos << ": boolean" << std::endl;
        }
    }
}
