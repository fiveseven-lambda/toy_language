#ifndef SYNTAX_HPP
#define SYNTAX_HPP

namespace syntax {
    class Expression {
    public:
        virtual ~Expression();
    };
    class Identifier : public Expression {
        std::string name;
    };
    enum class BinaryOperator {
        Add, Sub, Mul, Div
    };
}

#endif
