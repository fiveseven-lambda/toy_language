/**
 * @file error.hpp
 * @brief 各種エラーを定義する
 */

#ifndef ERROR_HPP
#define ERROR_HPP

#include <memory>

#include "pos.hpp"

//! 各種エラーを定義する．
namespace error {
    /**
     * @brief 全てのエラーの基底クラス．
     */
    class Error {
    public:
        virtual ~Error();
        /**
         * @brief 標準エラー出力でエラーの内容を説明する．
         * @param source ソースコードの文字列
         */
        void virtual eprint(const std::vector<std::string> &source) const = 0;
    };

    /**
     * @brief エラーを生成するときのヘルパ関数．
     *
     * `T` が `Error` の派生クラスのとき，`make<T>(...)` は `T` のコンストラクタを呼び出し `std::unique_ptr<Error>` にして返す．
     */
    template<class Err, class... Args>
    std::unique_ptr<Error> make(Args&&... args){
        return std::make_unique<Err>(std::forward<Args>(args)...);
    }

    /**
     * @brief 字句解析中に予期せぬ文字が現れた．
     *
     * 空白とコメント以外に，トークンの始まりとして適さない文字があった．
     */
    class UnexpectedCharacter : public Error {
        pos::Pos pos;
    public:
        UnexpectedCharacter(pos::Pos);
        void eprint(const std::vector<std::string> &) const override;
    };

    /**
     * @brief コメントが終了しなかった．
     *
     * コメントの途中で EOF に達した．
     */
    class UnterminatedComment : public Error {
        std::vector<pos::Pos> poss;
    public:
        UnterminatedComment(std::vector<pos::Pos>);
        void eprint(const std::vector<std::string> &) const override;
    };

    //! 整数リテラルの値が符号付き 32 ビット整数型におさまらなかった．
    class InvalidIntegerLiteral : public Error {
        std::exception &error;
        pos::Range pos;
    public:
        InvalidIntegerLiteral(std::exception &, pos::Range);
        void eprint(const std::vector<std::string> &) const override;
    };

    //! 開き括弧に対応する閉じ括弧が来ることなく EOF
    class NoClosingParenthesis : public Error {
        // 開いた場所
        pos::Range pos;
    public:
        NoClosingParenthesis(pos::Range);
        void eprint(const std::vector<std::string> &) const override;
    };

    //! 開き括弧に対応する閉じ括弧が無く，代わりに予期せぬトークンがある
    class UnexpectedTokenInParenthesis : public Error {
        pos::Range pos, open;
    public:
        UnexpectedTokenInParenthesis(pos::Range, pos::Range);
        void eprint(const std::vector<std::string> &) const override;
    };

    //! 括弧の中身が空
    class EmptyParenthesis : public Error {
        pos::Range open, close;
    public:
        EmptyParenthesis(pos::Range, pos::Range);
        void eprint(const std::vector<std::string> &) const override;
    };

    //! prefix の直後に予期せぬ EOF
    class UnexpectedEOFAfterPrefix : public Error {
        // prefix の場所
        pos::Range pos;
    public:
        UnexpectedEOFAfterPrefix(pos::Range);
        void eprint(const std::vector<std::string> &) const override;
    };

    //! prefix の直後に予期せぬトークン
    class UnexpectedTokenAfterPrefix : public Error {
        pos::Range pos_token, pos_prefix;
    public:
        UnexpectedTokenAfterPrefix(pos::Range, pos::Range);
        void eprint(const std::vector<std::string> &) const override;
    };

    //! 2 項演算子の後に式が来なかった
    class NoExpressionAfterOperator : public Error {
        pos::Range pos;
    public:
        NoExpressionAfterOperator(pos::Range);
        void eprint(const std::vector<std::string> &) const override;
    };

    //! 関数呼び出しにおいて，引数を区切る `,` の前に要素が無かった
    class EmptyArgument: public Error {
        pos::Range pos;
    public:
        EmptyArgument(pos::Range);
        void eprint(const std::vector<std::string> &) const override;
    };
}

#endif
