#include "type.hpp"

namespace type {
    Type::~Type() = default;

    llvm::Type *Integer::llvm_type(context::Context &context){
        return context.integer_type;
    }
    llvm::Type *Boolean::llvm_type(context::Context &context){
        return context.boolean_type;
    }
    llvm::Constant *Integer::default_value(context::Context &context){
        return llvm::ConstantInt::get(context.integer_type, 0);
    }
    llvm::Constant *Boolean::default_value(context::Context &context){
        return llvm::ConstantInt::get(context.boolean_type, 0);
    }
    llvm::Value *Integer::convert_from(std::shared_ptr<Type> type, llvm::Value *value, context::Context &context){
        return type->convert_into_integer(value, context);
    }
    llvm::Value *Boolean::convert_from(std::shared_ptr<Type> type, llvm::Value *value, context::Context &context){
        return type->convert_into_boolean(value, context);
    }
    llvm::Value *Integer::convert_into_integer(llvm::Value *value, context::Context &){
        return value;
    }
    llvm::Value *Boolean::convert_into_integer(llvm::Value *value, context::Context &context){
        return context.builder.CreateZExt(value, context.integer_type);
    }
    llvm::Value *Integer::convert_into_boolean(llvm::Value *value, context::Context &context){
    }
    llvm::Value *Boolean::convert_into_boolean(llvm::Value *value, context::Context &){
        return value;
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
