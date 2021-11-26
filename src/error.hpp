#ifndef ERROR_HPP
#define ERROR_HPP

#include <vector>
#include <string>
#include <memory>

#include "pos.hpp"

namespace error {
    class Error {
    public:
        void virtual eprint(const std::vector<std::string> &) const = 0;
        virtual ~Error();
    };

    template<class Err, class... Args>
    std::unique_ptr<Error> make(Args&&... args){
        return std::make_unique<Err>(std::forward<Args>(args)...);
    }

    class UnexpectedCharacter : public Error {
        pos::Pos pos;
    public:
        UnexpectedCharacter(pos::Pos &&);
        void eprint(const std::vector<std::string> &) const override;
    };

    class UnterminatedComment : public Error {
        std::vector<pos::Pos> poss;
    public:
        UnterminatedComment(std::vector<pos::Pos> &&);
        void eprint(const std::vector<std::string> &) const override;
    };
}

#endif
