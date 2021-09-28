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
    Invocation::Invocation(std::unique_ptr<Expression> function, std::vector<std::unique_ptr<Expression>> arguments):
        function(std::move(function)),
        arguments(std::move(arguments)) {}

    Precedence precedence(BinaryOperator binary_operator){
        switch(binary_operator){
            case BinaryOperator::Add:
            case BinaryOperator::Sub:
                return AddSubPrecedence;
            case BinaryOperator::Mul:
            case BinaryOperator::Div:
                return MulDivPrecedence;
            case BinaryOperator::Equal:
            case BinaryOperator::NotEqual:
                return EqualPrecedence;
        }
    }

    std::optional<std::string> Expression::into_identifier(){ return std::nullopt; }
    std::optional<std::string> Identifier::into_identifier(){ return std::move(name); }

    Type::~Type() = default;

    Sentence::~Sentence() = default;
    ExpressionSentence::ExpressionSentence(std::unique_ptr<Expression> expression):
        expression(std::move(expression)) {}
    Declaration::Declaration(std::string name, std::unique_ptr<Type> type, std::unique_ptr<Expression> expression):
        name(std::move(name)),
        type(std::move(type)),
        expression(std::move(expression)) {}
    Substitution::Substitution(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right):
        left(std::move(left)),
        right(std::move(right)) {}
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
            case UnaryOperator::Minus: name = "minus"; break;
            case UnaryOperator::Not: name = "not";
        }
        for(int i = 0; i < indent; ++i) std::cout << TAB;
        std::cout << "Unary(" << name << ")" << std::endl;
        operand->print(indent + 1);
    }
    void Binary::print(int indent){
        std::string name;
        switch(binary_operator){
            case BinaryOperator::Add: name = "add"; break;
            case BinaryOperator::Sub: name = "sub"; break;
            case BinaryOperator::Mul: name = "mul"; break;
            case BinaryOperator::Div: name = "div"; break;
            case BinaryOperator::Equal: name = "equal"; break;
            case BinaryOperator::NotEqual: name = "not equal";
        }
        for(int i = 0; i < indent; ++i) std::cout << TAB;
        std::cout << "Binary(" << name << ")" << std::endl;
        left->print(indent + 1);
        right->print(indent + 1);
    }
    void Invocation::print(int indent){
        for(int i = 0; i < indent; ++i) std::cout << TAB;
        std::cout << "Invocation" << std::endl;
        function->print(indent + 1);
        for(int i = 0; i < indent; ++i) std::cout << TAB;
        std::cout << "arguments: " << std::endl;
        for(auto &argument: arguments){
            argument->print(indent + 1);
        }
    }
    void Int::print(int indent){
        for(int i = 0; i < indent; ++i) std::cout << TAB;
        std::cout << "int" << std::endl;
    }
    void Float::print(int indent){
        for(int i = 0; i < indent; ++i) std::cout << TAB;
        std::cout << "float" << std::endl;
    }
    void Bool::print(int indent){
        for(int i = 0; i < indent; ++i) std::cout << TAB;
        std::cout << "bool" << std::endl;
    }
    void ExpressionSentence::print(int indent){
        for(int i = 0; i < indent; ++i) std::cout << TAB;
        std::cout << "Expression" << std::endl;
        if(expression) expression->print(indent + 1);
    }
    void Declaration::print(int indent){
        for(int i = 0; i < indent; ++i) std::cout << TAB;
        std::cout << "Declaration(" << name << ")" << std::endl;
        if(type) type->print(indent + 1);
        if(expression) expression->print(indent + 1);
    }
    void Substitution::print(int indent){
        for(int i = 0; i < indent; ++i) std::cout << TAB;
        std::cout << "Substitution" << std::endl;
        if(left) left->print(indent + 1);
        if(right) right->print(indent + 1);
    }
}
