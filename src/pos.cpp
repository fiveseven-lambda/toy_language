#include "pos.hpp"

namespace pos {
    //! デフォルトコンストラクタ
    /**
     * `line`，`byte` をともに 0 で初期化する．
     */
    Pos::Pos(): line(0), byte(0) {}

    //! コンストラクタ
    /**
     * @param line 何行目か（0-indexed で）
     * @param byte 何バイト目（0-indexed で）
     */
    Pos::Pos(std::size_t line, std::size_t byte): line(line), byte(byte) {}

    //! デフォルトコンストラクタ
    /**
     * 各値を 0 で初期化する．
     */
    Range::Range() = default;

    //! 複数行にまたがらない場合のコンストラクタ
    /**
     * @param line 何行目か（0-indexed で）
     * @param start 何バイト目からか（0-indexed で，`start` バイト目自身も含む）
     * @param end 何バイト目の手前までか（0-indexed で，`end` バイト目自身は含まない）
     */
    Range::Range(std::size_t line, std::size_t start, std::size_t end):
        start(line, start),
        end(line, end) {}

    Range::Range(Range &&) = default;
    Range &Range::operator=(Range &&) = default;

    //! Pos から `line`，`byte` の値を取り出す．
    std::pair<std::size_t, std::size_t> Pos::into_inner() const {
        return {line, byte};
    }

    Range &Range::operator+=(const Range &other){
        end = other.end;
        return *this;
    }

    //! `line`，`byte` の値を 1-indexed に直して出力．
    std::ostream &operator<<(std::ostream &os, const Pos &pos){
        return os << pos.line + 1 << ":" << pos.byte + 1;
    }

    //! 開始と終了の `line`，`byte` を 1-indexed・閉区間に直して出力．
    std::ostream &operator<<(std::ostream &os, const Range &range){
        auto [sline, sbyte] = range.start.into_inner();
        auto [eline, ebyte] = range.end.into_inner();
        return os
            << sline + 1 << ":" << sbyte + 1
            << "-" << eline + 1 << ":" << ebyte;
    }

    //! ソースコードから当該部分を切り出して出力する．
    /**
     * @param source ソースコードの文字列
     */
    void Pos::eprint(const std::vector<std::string> &source) const {
        std::cerr
            << source[line].substr(0, byte)
            << " !-> "
            << source[line].substr(byte)
            << std::endl;
    }

    //! ソースコードから該当部分を切り出して出力する．
    /**
     * @param source ソースコードの文字列
     */
    void Range::eprint(const std::vector<std::string> &source) const {
        auto [sline, sbyte] = start.into_inner();
        auto [eline, ebyte] = end.into_inner();
        if(sline == eline){
            std::cerr
                << source[sline].substr(0, sbyte)
                << " !-> "
                << source[sline].substr(sbyte, ebyte - sbyte)
                << " <-! "
                << source[sline].substr(ebyte)
                << std::endl;
        } else {
            std::cerr
                << source[sline].substr(0, sbyte)
                << " !-> "
                << source[sline].substr(sbyte)
                << std::endl
                << " ("
                << eline - sline - 1
                << ")"
                << std::endl
                << source[eline].substr(0, ebyte)
                << " <-! "
                << source[eline].substr(ebyte)
                << std::endl;
        }
    }
}
