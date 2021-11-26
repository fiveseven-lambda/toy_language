#include "error.hpp"

namespace error {
    Error::~Error() = default;

    UnexpectedCharacter::UnexpectedCharacter(pos::Pos &&pos): pos(std::move(pos)) {}
    void UnexpectedCharacter::eprint(const std::vector<std::string> &log) const {
        std::cerr << "unexpected character at " << pos << std::endl;
        pos.eprint(log);
    }
}
