#include "error.hpp"

namespace error {
    Error::~Error() = default;
    UnexpectedCharacter::UnexpectedCharacter(pos::Pos &&pos): pos(std::move(pos)) {}
    UnterminatedComment::UnterminatedComment(std::vector<pos::Pos> &&poss): poss(std::move(poss)) {}
    void UnexpectedCharacter::eprint(const std::vector<std::string> &log) const {
        std::cerr << "unexpected character at " << pos << std::endl;
        pos.eprint(log);
    }
    void UnterminatedComment::eprint(const std::vector<std::string> &log) const {
        std::cerr << "unterminated comment" << std::endl;
        for(const pos::Pos &pos : poss){
            std::cerr << "started at " << pos << std::endl;
            pos.eprint(log);
        }
    }
}
