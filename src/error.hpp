#ifndef ERROR_HPP
#define ERROR_HPP

#include <vector>
#include <string>
#include <memory>

#include "pos.hpp"

namespace error {
    class Error {
    public:
        virtual ~Error();
        void virtual eprint(const std::vector<std::string> &) const = 0;
    };

    template<class Err, class... Args>
    std::unique_ptr<Error> make(Args&&... args){
        return std::make_unique<Err>(std::forward<Args>(args)...);
    }

    class UnexpectedCharacter : public Error {
        pos::Pos pos;
    public:
        UnexpectedCharacter(pos::Pos);
        void eprint(const std::vector<std::string> &) const override;
    };

    class UnterminatedComment : public Error {
        std::vector<pos::Pos> poss;
    public:
        UnterminatedComment(std::vector<pos::Pos>);
        void eprint(const std::vector<std::string> &) const override;
    };

    class InvalidIntegerLiteral : public Error {
        std::exception &error;
        pos::Range pos;
    public:
        InvalidIntegerLiteral(std::exception &, pos::Range);
        void eprint(const std::vector<std::string> &) const override;
    };

    class NoExpressionAfterOperator : public Error {
        pos::Range pos;
    public:
        NoExpressionAfterOperator(pos::Range);
        void eprint(const std::vector<std::string> &) const override;
    };

    class NoClosingParenthesis : public Error {
        pos::Range pos;
    public:
        NoClosingParenthesis(pos::Range);
        void eprint(const std::vector<std::string> &) const override;
    };

    class UnexpectedTokenInParenthesis : public Error {
        pos::Range pos, open;
    public:
        UnexpectedTokenInParenthesis(pos::Range, pos::Range);
        void eprint(const std::vector<std::string> &) const override;
    };

    class EmptyParenthesis : public Error {
        pos::Range pos;
    public:
        EmptyParenthesis(pos::Range);
        void eprint(const std::vector<std::string> &) const override;
    };

    class EmptyArgument: public Error {
        pos::Range pos;
    public:
        EmptyArgument(pos::Range);
        void eprint(const std::vector<std::string> &) const override;
    };
}

#endif
