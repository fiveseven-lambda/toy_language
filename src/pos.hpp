#ifndef POS_HPP
#define POS_HPP

#include <cstddef>
#include <iostream>
#include <vector>
#include <string>
#include <utility>

namespace pos {
    class Pos {
        std::size_t line, byte;
    public:
        Pos();
        Pos(std::size_t, std::size_t);
        std::pair<std::size_t, std::size_t> into_inner() const;
        void eprint(const std::vector<std::string> &) const;
    };

    class Range {
        Pos start, end;
    public:
        Range();
        Range(Pos, Pos);
        void eprint(const std::vector<std::string> &) const;
    };
}

#endif
