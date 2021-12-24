#ifndef POS_HPP
#define POS_HPP

#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>
#include <string>

namespace pos {
    class Pos {
        std::size_t line, byte;
    public:
        Pos();
        Pos(std::size_t, std::size_t);
        std::pair<std::size_t, std::size_t> into_inner() const;
        friend std::ostream &operator<<(std::ostream &, const Pos &);
        void eprint(const std::vector<std::string> &) const;
    };

    class Range {
        Pos start, end;
    public:
        Range();
        Range(std::size_t, std::size_t, std::size_t);
        friend std::ostream &operator<<(std::ostream &, const Range &);
        void eprint(const std::vector<std::string> &) const;
    };
}

#endif
