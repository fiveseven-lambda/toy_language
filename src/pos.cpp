#include "pos.hpp"

namespace pos {
    /**
     * @brief デフォルトコンストラクタ
     *
     * `line`，`byte` をともに 0 で初期化する．
     */
    Pos::Pos(): line(0), byte(0) {}

    /**
     * @brief コンストラクタ
     * @param line 何行目か（0-indexed で）
     * @param byte 何バイト目か（0-indexed で）
     */
    Pos::Pos(std::size_t line, std::size_t byte): line(line), byte(byte) {}

    /**
     * @brief デフォルトコンストラクタ
     *
     * 各値を 0 で初期化する．
     */
    Range::Range() = default;

    /**
     * @brief コンストラクタ
     *
     * @param start 開始（自身含む）
     * @param end 終了（自身含まない）
     */
    Range::Range(Pos start, Pos end): start(start), end(end) {}

    /**
     * @brief コンストラクタ
     *
     * 複数行にまたがらない場合（開始と終了が同じ行にある場合）に使う．
     * @param line 何行目か（0-indexed で）
     * @param start 何バイト目からか（0-indexed で，`start` 自身も含む）
     * @param end 何バイト目の手前までか（0-indexed で，`end` 自身は含まない）
     */
    Range::Range(std::size_t line, std::size_t start, std::size_t end):
        start(line, start),
        end(line, end) {}

    /**
     * @brief ムーブコンストラクタ
     */
    Range::Range(Range &&) = default;

    /**
     * @brief ムーブ代入演算子
     */
    Range &Range::operator=(Range &&) = default;

    /**
     * @brief クローン
     */
    Range Range::clone(){
        return Range(start, end);
    }

    /**
     * @brief Pos から `line`，`byte` の値を取り出す．
     * @return `first` が `line`，`second` が `byte`．
     */
    std::pair<std::size_t, std::size_t> Pos::into_inner() const {
        return {line, byte};
    }

    /**
     * @brief 範囲を結合する．
     */
    Range &Range::operator+=(const Range &other){
        end = other.end;
        return *this;
    }

    /**
     * @brief `line`，`byte` の値を 1-indexed に直して出力する．
     */
    std::ostream &operator<<(std::ostream &os, const Pos &pos){
        return os << pos.line + 1 << ":" << pos.byte + 1;
    }
    /**
     * @brief 開始と終了の `line`，`byte` を 1-indexed，閉区間に直して出力する．
     */
    std::ostream &operator<<(std::ostream &os, const Range &range){
        auto [sline, sbyte] = range.start.into_inner();
        auto [eline, ebyte] = range.end.into_inner();
        return os
            << sline + 1 << ":" << sbyte + 1
            << "-" << eline + 1 << ":" << ebyte;
    }

    /**
     * @brief ソースコードから当該の行を切り出して出力する．
     * @param source ソースコード（文字列）
     */
    void Pos::eprint(const std::vector<std::string> &source) const {
        std::cerr
            << source[line].substr(0, byte)
            << " !-> "
            << source[line].substr(byte)
            << std::endl;
    }
    /**
     * @brief ソースコードから当該の範囲の前後を切り出して出力する．
     * @param source ソースコード（文字列）
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
