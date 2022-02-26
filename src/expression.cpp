#include "expression.hpp"

#include <iostream>
#include <string_view>

namespace expression {
    Expression::~Expression() = default;
    //! コンストラクタ
    Identifier::Identifier(std::string name): name(std::move(name)) {}
    //! コンストラクタ
    Integer::Integer(std::int32_t value): value(value) {}
    //! コンストラクタ
    UnaryOperation::UnaryOperation(
        UnaryOperator unary_operator,
        std::unique_ptr<Expression> operand
    ):
        unary_operator(unary_operator),
        operand(std::move(operand)) {}
    //! コンストラクタ
    BinaryOperation::BinaryOperation(
        BinaryOperator binary_operator,
        std::unique_ptr<Expression> left,
        std::unique_ptr<Expression> right
    ):
        binary_operator(binary_operator),
        left(std::move(left)),
        right(std::move(right)) {}
    //! コンストラクタ
    Invocation::Invocation(
        std::unique_ptr<Expression> function,
        std::vector<std::unique_ptr<Expression>> arguments
    ):
        function(std::move(function)),
        arguments(std::move(arguments)) {}

    static constexpr std::string_view INDENT = "    ";
    void Identifier::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": Identifier(" << name << ")" << std::endl;
    }
    void Integer::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": Integer(" << value << ")" << std::endl;
    }
    void UnaryOperation::debug_print(int depth) const {
        std::string_view name;
        switch(unary_operator){
            case UnaryOperator::Plus: name = "plus"; break;
            case UnaryOperator::Minus: name = "minus"; break;
            case UnaryOperator::LogicalNot: name = "logical not"; break;
            case UnaryOperator::BitNot: name = "bitwise not";
        }
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": UnaryOperation(" << name << ")" << std::endl;
        operand->debug_print(depth + 1);
    }
    void BinaryOperation::debug_print(int depth) const {
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
        std::cout << pos << ": BinaryOperation(" << name << ")" << std::endl;
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
