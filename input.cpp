#include "input.hpp"

#include <iostream>

constexpr char PROMPT[] = "> ";

Input::Input(std::istream &source, bool prompt):
    source(source),
    prompt(prompt)
{
    if(prompt) std::cout << PROMPT;
}

int Input::peek() {
    return source.peek();
}
int Input::get() {
    int ret = source.get();
    if(ret == '\n' && prompt){
        std::cout << PROMPT;
    }
    return ret;
}
int Input::get_if(char c){
    bool ret = source.peek() == c;
    if(ret) get();
    return ret;
}