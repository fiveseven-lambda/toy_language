#include "type.hpp"

namespace type {
    Type::~Type() = default;

    llvm::Type *Integer::llvm_type(context::Context &context){
        return context.integer_type;
    }
    llvm::Type *Boolean::llvm_type(context::Context &context){
        return context.boolean_type;
    }

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
