#include "syntax.hpp"

namespace syntax {
    Expression::~Expression() = default;
    Identifier::Identifier(std::string name):
        name(std::move(name)) {}
    Integer::Integer(std::int32_t value):
        value(value) {}
    Unary::Unary(UnaryOperator unary_operator, std::unique_ptr<Expression> operand):
        unary_operator(unary_operator),
        operand(std::move(operand)) {}
    Binary::Binary(BinaryOperator binary_operator, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right):
        binary_operator(binary_operator),
        left(std::move(left)),
        right(std::move(right)) {}

    Precedence precedence(BinaryOperator binary_operator){
        switch(binary_operator){
            case BinaryOperator::Add:
            case BinaryOperator::Sub:
                return AddSubPrecedence;
            case BinaryOperator::Mul:
            case BinaryOperator::Div:
                return MulDivPrecedence;
        }
    }
}

#include <iostream>
namespace syntax {
    static constexpr char TAB[] = "    ";
    void Identifier::print(int indent){
        for(int i = 0; i < indent; ++i) std::cout << TAB;
        std::cout << "Identifier(" << name << ")" << std::endl;
    }
    void Integer::print(int indent){
        for(int i = 0; i < indent; ++i) std::cout << TAB;
        std::cout << "Integer(" << value << ")" << std::endl;
    }
    void Unary::print(int indent){
        std::string name;
        switch(unary_operator){
            case UnaryOperator::Plus: name = "plus"; break;
            case UnaryOperator::Minus: name = "minus";
        }
        for(int i = 0; i < indent; ++i) std::cout << TAB;
        std::cout << "Unary(" << name << ")" << std::endl;
        operand->print(indent + 1);
    }
    void Binary::print(int indent){
        left->print(indent + 1);
        std::string name;
        switch(binary_operator){
            case BinaryOperator::Add: name = "add"; break;
            case BinaryOperator::Sub: name = "sub"; break;
            case BinaryOperator::Mul: name = "mul"; break;
            case BinaryOperator::Div: name = "div";
        }
        for(int i = 0; i < indent; ++i) std::cout << TAB;
        std::cout << "Binary(" << name << ")" << std::endl;
        right->print(indent + 1);
    }
}
