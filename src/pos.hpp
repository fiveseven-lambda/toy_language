#ifndef POS_HPP
#define POS_HPP

#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>
#include <string>

namespace pos {
    // ソースコード上の文字の位置
    class Pos {
        // 何行目の
        std::size_t line;
        // 何バイト目
        std::size_t byte;
    public:
        // デフォルトコンストラクタは 0 で初期化
        Pos();
        // line, byte をこの順で渡す
        Pos(std::size_t, std::size_t);
        // line, byte の組を返す
        std::pair<std::size_t, std::size_t> into_inner() const;
        // エラーメッセージで何行目何バイト目か表示
        friend std::ostream &operator<<(std::ostream &, const Pos &);
        // エラーメッセージにソースコードの一部を含める
        void eprint(const std::vector<std::string> &) const;
    };

    // ソースコード上の式や文の範囲
    class Range {
        // どこから
        Pos start;
        // どこまで
        Pos end;
    public:
        // すべて 0 で初期化
        Range();
        // 何行目，何バイト目から何バイト目？
        Range(std::size_t, std::size_t, std::size_t);
        Range(const Range &) = delete;
        Range &operator=(const Range &) = delete;
        Range(Range &&);
        Range &operator=(Range &&);
        // 範囲の和
        Range &operator+=(const Range &);
        // エラーメッセージで何行目何バイト目から何行目何バイト目までか表示
        friend std::ostream &operator<<(std::ostream &, const Range &);
        // エラーメッセージにソースコードの一部を含める
        void eprint(const std::vector<std::string> &) const;
    };
}

#endif
