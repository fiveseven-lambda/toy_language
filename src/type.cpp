/**
 * @file type.cpp
 */
#include "type.hpp"

namespace type {
    Type::~Type() = default;

    std::unique_ptr<value::Type> Integer::into() const {
        return std::make_unique<value::Integer>();
    }
    std::unique_ptr<value::Type> Boolean::into() const {
        return std::make_unique<value::Boolean>();
    }

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
