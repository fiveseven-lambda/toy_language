#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>
#include <memory>
#include "token.hpp"

namespace error {
    class Error {
    public:
        void virtual print() = 0;
        virtual ~Error();
    };

    template<class Err, class... Args>
    std::unique_ptr<Error> make(Args&&... args){
        return std::make_unique<Err>(std::forward<Args>(args)...);
    }

    class UnexpectedCharacter : public Error {
        char c;
    public:
        UnexpectedCharacter(char);
        void print() override;
    };

    class NoClosingParenthesis : public Error {
    public:
        void print() override;
    };

    class SyntaxError : public Error {
    public:
        void print() override;
    };
}

#endif
