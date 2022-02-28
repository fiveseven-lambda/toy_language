/**
 * @file error.cpp
 */
#include "error.hpp"

namespace error {
    Error::~Error() = default;
    //! コンストラクタ
    UnexpectedCharacter::UnexpectedCharacter(pos::Pos pos): pos(std::move(pos)) {}
    //! コンストラクタ
    UnterminatedComment::UnterminatedComment(std::vector<pos::Pos> poss): poss(std::move(poss)) {}
    //! コンストラクタ
    InvalidIntegerLiteral::InvalidIntegerLiteral(std::exception &error, pos::Range pos): error(error), pos(std::move(pos)) {}
    //! コンストラクタ
    NoClosingParenthesis::NoClosingParenthesis(pos::Range pos): pos(std::move(pos)) {}
    //! コンストラクタ
    UnexpectedTokenInParenthesis::UnexpectedTokenInParenthesis(pos::Range pos, pos::Range open): pos(std::move(pos)), open(std::move(open)) {}
    //! コンストラクタ
    EmptyParenthesis::EmptyParenthesis(pos::Range pos): pos(std::move(pos)) {}
    //! コンストラクタ
    UnexpectedTokenAtFactor::UnexpectedTokenAtFactor(pos::Range pos): pos(std::move(pos)) {}
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
        std::cerr << "empty parenthesis (opened at " << pos << ")" << std::endl;
        pos.eprint(log);
    }
    void UnexpectedTokenAtFactor::eprint(const std::vector<std::string> &log) const {
        std::cerr << "unexpected token at " << pos << " (expected a factor)" << std::endl;
        pos.eprint(log);
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
