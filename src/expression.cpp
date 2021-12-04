#include <iostream>
#include <string_view>

#include "expression.hpp"

namespace expression {
    Expression::~Expression() = default;
    Identifier::Identifier(std::string name): name(std::move(name)) {}
    Integer::Integer(std::int32_t value): value(value) {}
    Unary::Unary(UnaryOperator unary_operator, std::unique_ptr<Expression> operand):
        unary_operator(unary_operator),
        operand(std::move(operand)) {}
    Binary::Binary(BinaryOperator binary_operator, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right):
        binary_operator(binary_operator),
        left(std::move(left)),
        right(std::move(right)) {}
    Invocation::Invocation(std::unique_ptr<Expression> function, std::vector<std::unique_ptr<Expression>> arguments):
        function(std::move(function)),
        arguments(std::move(arguments)) {}

    std::optional<std::string> Expression::identifier(){ return std::nullopt; }
    std::optional<std::string> Identifier::identifier(){ return std::move(name); }

    std::pair<std::shared_ptr<type::Type>, llvm::Value *> Identifier::rvalue(context::Context &context, const std::map<std::string, std::pair<std::shared_ptr<type::Type>, llvm::Value *>> &variables){
        auto [type, value] = variables.find(name)->second;
        return {type, context.builder.CreateLoad(type->llvm_type(context), value)};
    }
    std::pair<std::shared_ptr<type::Type>, llvm::Value *> Identifier::lvalue(context::Context &, const std::map<std::string, std::pair<std::shared_ptr<type::Type>, llvm::Value *>> &){}
    std::pair<std::shared_ptr<type::Type>, llvm::Value *> Integer::rvalue(context::Context &context, const std::map<std::string, std::pair<std::shared_ptr<type::Type>, llvm::Value *>> &){
        return {std::make_unique<type::Integer>(), llvm::ConstantInt::get(context.integer_type, value)};
    }
    std::pair<std::shared_ptr<type::Type>, llvm::Value *> Integer::lvalue(context::Context &, const std::map<std::string, std::pair<std::shared_ptr<type::Type>, llvm::Value *>> &){}
    std::pair<std::shared_ptr<type::Type>, llvm::Value *> Unary::rvalue(context::Context &context, const std::map<std::string, std::pair<std::shared_ptr<type::Type>, llvm::Value *>> &variables){
        auto [operand_type, operand_rvalue] = operand->rvalue(context, variables);
        std::shared_ptr<type::Type> type;
        switch(unary_operator){
            case UnaryOperator::LogicalNot:
                type = std::make_shared<type::Boolean>();
                break;
            case UnaryOperator::Plus:
            case UnaryOperator::Minus:
            case UnaryOperator::BitNot:
                type = std::make_shared<type::Integer>();
        }
        auto converted_operand = type->convert_from(operand_type, operand_rvalue, context);
        switch(unary_operator){
            case UnaryOperator::LogicalNot:
                return {type, context.builder.CreateNot(converted_operand)};
            case UnaryOperator::Plus:
                return {type, converted_operand};
            case UnaryOperator::Minus:
                return {type, context.builder.CreateNSWSub(llvm::ConstantInt::get(context.integer_type, 0), converted_operand)};
            case UnaryOperator::BitNot:
                return {type, context.builder.CreateXor(converted_operand, llvm::ConstantInt::get(context.integer_type, -1))};
        }
    }
    std::pair<std::shared_ptr<type::Type>, llvm::Value *> Unary::lvalue(context::Context &, const std::map<std::string, std::pair<std::shared_ptr<type::Type>, llvm::Value *>> &){}
    std::pair<std::shared_ptr<type::Type>, llvm::Value *> Binary::rvalue(context::Context &context, const std::map<std::string, std::pair<std::shared_ptr<type::Type>, llvm::Value *>> &variables){
        constexpr int lvalue_operations = 
            1 << static_cast<int>(BinaryOperator::Assign)
            | 1 << static_cast<int>(BinaryOperator::MulAssign)
            | 1 << static_cast<int>(BinaryOperator::DivAssign)
            | 1 << static_cast<int>(BinaryOperator::RemAssign)
            | 1 << static_cast<int>(BinaryOperator::AddAssign)
            | 1 << static_cast<int>(BinaryOperator::SubAssign)
            | 1 << static_cast<int>(BinaryOperator::LeftShiftAssign)
            | 1 << static_cast<int>(BinaryOperator::RightShiftAssign)
            | 1 << static_cast<int>(BinaryOperator::BitAndAssign)
            | 1 << static_cast<int>(BinaryOperator::BitOrAssign)
            | 1 << static_cast<int>(BinaryOperator::BitXorAssign);
        if(lvalue_operations >> static_cast<int>(binary_operator) & 1){
        }else{
            auto [left_type, left_value] = left->rvalue(context, variables);
            auto [right_type, right_value] = right->rvalue(context, variables);
            constexpr int integer_operations = 
                1 << static_cast<int>(BinaryOperator::Add)
                | 1 << static_cast<int>(BinaryOperator::Sub)
                | 1 << static_cast<int>(BinaryOperator::Mul)
                | 1 << static_cast<int>(BinaryOperator::Div)
                | 1 << static_cast<int>(BinaryOperator::Rem)
                | 1 << static_cast<int>(BinaryOperator::LeftShift)
                | 1 << static_cast<int>(BinaryOperator::RightShift)
                | 1 << static_cast<int>(BinaryOperator::BitAnd)
                | 1 << static_cast<int>(BinaryOperator::BitOr)
                | 1 << static_cast<int>(BinaryOperator::BitXor)
                | 1 << static_cast<int>(BinaryOperator::Equal)
                | 1 << static_cast<int>(BinaryOperator::NotEqual)
                | 1 << static_cast<int>(BinaryOperator::Less)
                | 1 << static_cast<int>(BinaryOperator::Greater)
                | 1 << static_cast<int>(BinaryOperator::LessEqual)
                | 1 << static_cast<int>(BinaryOperator::GreaterEqual);
            if(integer_operations >> static_cast<int>(binary_operator) & 1){
                auto integer_type = std::make_shared<type::Integer>();
                auto boolean_type = std::make_shared<type::Boolean>();
                auto left = integer_type->convert_from(left_type, left_value, context);
                auto right = integer_type->convert_from(right_type, right_value, context);
                switch(binary_operator){
                    case BinaryOperator::Add:
                        return {integer_type, context.builder.CreateNSWAdd(left, right)};
                    case BinaryOperator::Sub:
                        return {integer_type, context.builder.CreateNSWSub(left, right)};
                    case BinaryOperator::Mul:
                        return {integer_type, context.builder.CreateNSWMul(left, right)};
                    case BinaryOperator::Div:
                        return {integer_type, context.builder.CreateExactSDiv(left, right)};
                    case BinaryOperator::Rem:
                        return {integer_type, context.builder.CreateSRem(left, right)};
                    case BinaryOperator::Equal:
                        return {boolean_type, context.builder.CreateICmpEQ(left, right)};
                    case BinaryOperator::NotEqual:
                        return {boolean_type, context.builder.CreateICmpNE(left, right)};
                    case BinaryOperator::Less:
                        return {boolean_type, context.builder.CreateICmpSLT(left, right)};
                    case BinaryOperator::Greater:
                        return {boolean_type, context.builder.CreateICmpSGT(left, right)};
                    case BinaryOperator::LessEqual:
                        return {boolean_type, context.builder.CreateICmpSLE(left, right)};
                    case BinaryOperator::GreaterEqual:
                        return {boolean_type, context.builder.CreateICmpSGE(left, right)};
                }
            }
        }
    }
    std::pair<std::shared_ptr<type::Type>, llvm::Value *> Binary::lvalue(context::Context &, const std::map<std::string, std::pair<std::shared_ptr<type::Type>, llvm::Value *>> &){}
    std::pair<std::shared_ptr<type::Type>, llvm::Value *> Invocation::rvalue(context::Context &, const std::map<std::string, std::pair<std::shared_ptr<type::Type>, llvm::Value *>> &){}
    std::pair<std::shared_ptr<type::Type>, llvm::Value *> Invocation::lvalue(context::Context &, const std::map<std::string, std::pair<std::shared_ptr<type::Type>, llvm::Value *>> &){}

    static constexpr std::string_view INDENT = "    ";
    void Identifier::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": Identifier(" << name << ")" << std::endl;
    }
    void Integer::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": Integer(" << value << ")" << std::endl;
    }
    void Unary::debug_print(int depth) const {
        std::string_view name;
        switch(unary_operator){
            case UnaryOperator::Plus: name = "plus"; break;
            case UnaryOperator::Minus: name = "minus"; break;
            case UnaryOperator::LogicalNot: name = "logical not"; break;
            case UnaryOperator::BitNot: name = "bitwise not";
        }
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": Unary(" << name << ")" << std::endl;
        operand->debug_print(depth + 1);
    }
    void Binary::debug_print(int depth) const {
        std::string_view name;
        switch(binary_operator){
            case BinaryOperator::Mul: name = "mul"; break;
            case BinaryOperator::Div: name = "div"; break;
            case BinaryOperator::Rem: name = "rem"; break;
            case BinaryOperator::Add: name = "add"; break;
            case BinaryOperator::Sub: name = "sub"; break;
            case BinaryOperator::LeftShift: name = "left shift"; break;
            case BinaryOperator::RightShift: name = "right shift"; break;
            case BinaryOperator::BitAnd: name = "bitwise and"; break;
            case BinaryOperator::BitOr: name = "bitwise or"; break;
            case BinaryOperator::BitXor: name = "bitwise xor"; break;
            case BinaryOperator::Equal: name = "equal"; break;
            case BinaryOperator::NotEqual: name = "not equal"; break;
            case BinaryOperator::Less: name = "less"; break;
            case BinaryOperator::Greater: name = "greater"; break;
            case BinaryOperator::LessEqual: name = "less or equal to"; break;
            case BinaryOperator::GreaterEqual: name = "greater or equal to"; break;
            case BinaryOperator::LogicalAnd: name = "logical and"; break;
            case BinaryOperator::LogicalOr: name = "logical or"; break;
            case BinaryOperator::Assign: name = "assign"; break;
            case BinaryOperator::MulAssign: name = "mul assign"; break;
            case BinaryOperator::DivAssign: name = "div assign"; break;
            case BinaryOperator::RemAssign: name = "rem assign"; break;
            case BinaryOperator::AddAssign: name = "add assign"; break;
            case BinaryOperator::SubAssign: name = "sub assign"; break;
            case BinaryOperator::BitAndAssign: name = "bitwise and assign"; break;
            case BinaryOperator::BitOrAssign: name = "bitwise or assign"; break;
            case BinaryOperator::BitXorAssign: name = "bitwise xor assign"; break;
            case BinaryOperator::RightShiftAssign: name = "right shift assign"; break;
            case BinaryOperator::LeftShiftAssign: name = "left shift assign"; break;
        }
        left->debug_print(depth + 1);
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": Binary(" << name << ")" << std::endl;
        right->debug_print(depth + 1);
    }
    void Invocation::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": Invocation" << std::endl;
        function->debug_print(depth + 1);
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << "arguments: " << std::endl;
        for(auto &argument: arguments){
            argument->debug_print(depth + 1);
        }
    }
}
