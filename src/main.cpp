#include <iostream>
#include <string>

#include "pos.hpp"
#include "error.hpp"

using namespace std::literals::string_literals;

int main() {
    std::vector<std::string> log;
    log.push_back("abcde"s);
    log.push_back("ABCDE"s);
    log.push_back("12345"s);
    pos::Range(pos::Pos(0, 2), pos::Pos(2, 4)).eprint(log);
}
