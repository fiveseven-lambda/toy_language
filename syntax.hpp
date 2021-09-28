#ifndef SYNTAX_HPP
#define SYNTAX_HPP

#include <cstdlib>
#include <string>
#include <memory>
#include <vector>
#include <optional>

namespace syntax {
    class Expression {
    public:
        virtual ~Expression();
        virtual std::optional<std::string> into_identifier();
        virtual void print(int = 0) = 0;
    };
    class Identifier : public Expression {
        std::string name;
    public:
        Identifier(std::string);
        std::optional<std::string> into_identifier() override;
        void print(int) override;
    };
    class Integer : public Expression {
        std::int32_t value;
    public:
        Integer(std::int32_t);
        void print(int) override;
    };
    enum class UnaryOperator {
        Plus, Minus,
        Not
    };
    class Unary : public Expression {
        UnaryOperator unary_operator;
        std::unique_ptr<Expression> operand;
    public:
        Unary(UnaryOperator, std::unique_ptr<Expression>);
        void print(int) override;
    };
    enum class BinaryOperator {
        Add, Sub, Mul, Div,
        Equal, NotEqual
    };
    enum Precedence {
        AddSubPrecedence,
        MulDivPrecedence,
        ComparePrecedence,
        EqualPrecedence,
        MaxPrecedence
    };
    Precedence precedence(BinaryOperator);
    class Binary : public Expression {
        BinaryOperator binary_operator;
        std::unique_ptr<Expression> left, right;
    public:
        Binary(BinaryOperator, std::unique_ptr<Expression>, std::unique_ptr<Expression>);
        void print(int) override;
    };
    class Invocation : public Expression {
        std::unique_ptr<Expression> function;
        std::vector<std::unique_ptr<Expression>> arguments;
    public:
        Invocation(std::unique_ptr<Expression>, std::vector<std::unique_ptr<Expression>>);
        void print(int) override;
    };

    class Type {
    public:
        virtual ~Type();
        virtual void print(int) = 0;
    };
    class Int : public Type {
        void print(int) override;
    };
    class Float : public Type {
        void print(int) override;
    };
    class Bool : public Type {
        void print(int) override;
    };

    class Sentence {
    public:
        virtual ~Sentence();
        virtual void print(int = 0) = 0;
    };
    class ExpressionSentence : public Sentence {
        std::unique_ptr<Expression> expression;
    public:
        ExpressionSentence(std::unique_ptr<Expression>);
        void print(int) override;
    };
    class Declaration : public Sentence {
        std::string name;
        std::unique_ptr<Type> type;
        std::unique_ptr<Expression> expression;
    public:
        Declaration(std::string, std::unique_ptr<Type>, std::unique_ptr<Expression>);
        void print(int) override;
    };
    class Substitution : public Sentence {
        std::unique_ptr<Expression> left, right;
    public:
        Substitution(std::unique_ptr<Expression>, std::unique_ptr<Expression>);
        void print(int) override;
    };
}

#endif
