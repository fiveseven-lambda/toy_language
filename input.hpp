#ifndef INPUT_HPP
#define INPUT_HPP

#include <istream>

class Input {
    std::istream &source;
    bool prompt;
public:
    Input(std::istream &, bool);
    int peek(), get(), get_if(char);
};

#endif
