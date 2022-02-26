#ifndef POS_HPP
#define POS_HPP

#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>
#include <string>

/**
 * @file pos.hpp
 * @brief エラー報告に位置情報をもたせるためのクラス群を定義する．
 */

//! エラー報告に位置情報をもたせるためのクラス群
namespace pos {
    //! ソースコード中の文字の位置
    /**
     * `line` 行目，`byte` バイト目の組で表す．
     */
    class Pos {
        std::size_t line, byte;
    public:
        Pos();
        Pos(std::size_t, std::size_t);
        std::pair<std::size_t, std::size_t> into_inner() const;
        friend std::ostream &operator<<(std::ostream &, const Pos &);
        void eprint(const std::vector<std::string> &) const;
    };

    //! トークンや式など，複数文字にわたるものの位置
    /**
     * `start` から `end` の手前まで（半開区間）の形で表す．
     */
    class Range {
        Pos start, end;
    public:
        Range();
        Range(std::size_t, std::size_t, std::size_t);
        Range(const Range &) = delete;
        Range &operator=(const Range &) = delete;
        Range(Range &&);
        Range &operator=(Range &&);
        // 範囲の和
        Range &operator+=(const Range &);
        friend std::ostream &operator<<(std::ostream &, const Range &);
        void eprint(const std::vector<std::string> &) const;
    };
}

#endif
