/**
 * @file type.cpp
 */
#include "type.hpp"

namespace type {
    Type::~Type() = default;

    static constexpr std::string_view INDENT = "    ";
    void Integer::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": Integer" << std::endl;
    }
    void Boolean::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": Boolean" << std::endl;
    }
}
