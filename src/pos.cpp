#include "pos.hpp"

namespace pos {
    Pos::Pos() = default;
    Pos::Pos(std::size_t line, std::size_t byte): line(line), byte(byte) {}
    Range::Range() = default;
    Range::Range(std::size_t line, std::size_t start, std::size_t end):
        start(line, start),
        end(line, end) {}

    std::pair<std::size_t, std::size_t> Pos::into_inner() const {
        return {line, byte};
    }

    std::ostream &operator<<(std::ostream &os, const Pos &pos){
        return os << pos.line + 1 << ":" << pos.byte + 1;
    }
    std::ostream &operator<<(std::ostream &os, const Range &range){
        auto [sline, sbyte] = range.start.into_inner();
        auto [eline, ebyte] = range.end.into_inner();
        return os
            << sline + 1 << ":" << sbyte + 1
            << "-" << eline + 1 << ":" << ebyte;
    }

    void Pos::eprint(const std::vector<std::string> &log) const {
        std::cerr
            << log[line].substr(0, byte)
            << " !-> "
            << log[line].substr(byte)
            << std::endl;
    }
    void Range::eprint(const std::vector<std::string> &log) const {
        auto [sline, sbyte] = start.into_inner();
        auto [eline, ebyte] = end.into_inner();
        if(sline == eline){
            std::cerr
                << log[sline].substr(0, sbyte)
                << " !-> "
                << log[sline].substr(sbyte, ebyte - sbyte)
                << " <-! "
                << log[sline].substr(ebyte)
                << std::endl;
        } else {
            std::cerr
                << log[sline].substr(0, sbyte)
                << " !-> "
                << log[sline].substr(sbyte)
                << std::endl
                << " ("
                << eline - sline - 1
                << ")"
                << std::endl
                << log[eline].substr(0, ebyte)
                << " <-! "
                << log[eline].substr(ebyte)
                << std::endl;
        }
    }
}
