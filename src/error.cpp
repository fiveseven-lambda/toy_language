/**
 * @file error.cpp
 */
#include "error.hpp"

namespace error {
    Error::~Error() = default;
    /**
     * @brief コンストラクタ
     * @param pos 予期せぬ文字のあった場所
     */
    UnexpectedCharacter::UnexpectedCharacter(pos::Pos pos): pos(std::move(pos)) {}
    /**
     * @brief コンストラクタ
     * @param poss コメントの開始位置．ネストしていた場合それら全て
     */
    UnterminatedComment::UnterminatedComment(std::vector<pos::Pos> poss): poss(std::move(poss)) {}
    /**
     * @brief コンストラクタ
     * @param error boost の safe_numerics が投げた例外
     * @param pos 整数リテラルの位置
     */
    InvalidIntegerLiteral::InvalidIntegerLiteral(std::exception &error, pos::Range pos): error(error), pos(std::move(pos)) {}
    /**
     * @brief コンストラクタ
     * @param pos_token 予期せぬトークンの位置
     * @param pos_prefix 前置単項演算子の位置
     */
    UnexpectedTokenAfterPrefix::UnexpectedTokenAfterPrefix(pos::Range pos_token, pos::Range pos_prefix):
        pos_token(std::move(pos_token)),
        pos_prefix(std::move(pos_prefix)) {}
    /**
     * @brief コンストラクタ
     * @param pos 開き括弧の位置
     */
    NoClosingParenthesis::NoClosingParenthesis(pos::Range pos): pos(std::move(pos)) {}
    /**
     * @brief コンストラクタ
     * @param pos 予期せぬトークンの位置
     * @param open 開き括弧の位置
     */
    UnexpectedTokenInParenthesis::UnexpectedTokenInParenthesis(pos::Range pos, pos::Range open): pos(std::move(pos)), open(std::move(open)) {}
    /**
     * @brief コンストラクタ
     * @param open 開き括弧の位置
     * @param close 閉じ括弧の位置
     */
    EmptyParenthesis::EmptyParenthesis(pos::Range open, pos::Range close):
        open(std::move(open)),
        close(std::move(close)) {}
    //! コンストラクタ
    UnexpectedEOFAfterPrefix::UnexpectedEOFAfterPrefix(pos::Range pos) : pos(std::move(pos)) {}
    /**
     * @brief コンストラクタ
     * @param pos 2 項演算子のあった場所
     */
    NoExpressionAfterOperator::NoExpressionAfterOperator(pos::Range pos): pos(std::move(pos)) {}
    /**
     * @brief コンストラクタ
     * @param pos カンマのあった場所
     */
    EmptyArgument::EmptyArgument(pos::Range pos): pos(std::move(pos)) {}

    void UnexpectedCharacter::eprint(const std::vector<std::string> &log) const {
        std::cerr << "unexpected character at " << pos << std::endl;
        pos.eprint(log);
    }
    void UnterminatedComment::eprint(const std::vector<std::string> &log) const {
        std::cerr << "unterminated comment" << std::endl;
        for(const pos::Pos &pos : poss){
            std::cerr << "started at " << pos << std::endl;
            pos.eprint(log);
        }
    }
    void InvalidIntegerLiteral::eprint(const std::vector<std::string> &log) const {
        std::cerr << "invalid integer literal (" << error.what() << ") at " << pos << std::endl;
        pos.eprint(log);
    }
    void UnexpectedTokenAfterPrefix::eprint(const std::vector<std::string> &log) const {
        std::cerr << "unexpected token at " << pos_token << std::endl;
        pos_token.eprint(log);
        std::cerr << "after prefix at " << pos_prefix << std::endl;
        pos_prefix.eprint(log);
    }
    void NoClosingParenthesis::eprint(const std::vector<std::string> &log) const {
        std::cerr << "no closing parenthesis (opened at " << pos << ")" << std::endl;
        pos.eprint(log);
    }
    void UnexpectedTokenInParenthesis::eprint(const std::vector<std::string> &log) const {
        std::cerr << "unexpected token at " << pos << std::endl;
        pos.eprint(log);
        std::cerr << "note: parenthesis opened at " << open << std::endl;
        open.eprint(log);
    }
    void EmptyParenthesis::eprint(const std::vector<std::string> &log) const {
        std::cerr << "empty parenthesis (opened at " << open << ")" << std::endl;
        open.eprint(log);
        std::cerr << "closed at " << close << ")" << std::endl;
        close.eprint(log);
    }
    void UnexpectedEOFAfterPrefix::eprint(const std::vector<std::string> &log) const {
        std::cerr << "unexpected end of file after the prefix at " << pos << std::endl;
        pos.eprint(log);
    }
    void NoExpressionAfterOperator::eprint(const std::vector<std::string> &log) const {
        std::cerr << "an expression expected after an operator at " << pos << std::endl;
        pos.eprint(log);
    }
    void EmptyArgument::eprint(const std::vector<std::string> &log) const {
        std::cerr << "empty argument in a function call at " << pos << std::endl;
        pos.eprint(log);
    }
}
