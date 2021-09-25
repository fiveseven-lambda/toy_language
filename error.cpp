#include "error.hpp"

#include <iostream>

namespace error {
    Error::~Error() = default;
    UnexpectedCharacter::UnexpectedCharacter(char c):
        c(c) {}
    void UnexpectedCharacter::print(){
        std::cerr << "unexpected character: " << c << std::endl;
    }
}
