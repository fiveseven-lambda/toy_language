#include "error.hpp"

namespace error {
    Error::~Error() = default;
    UnexpectedCharacter::UnexpectedCharacter(pos::Pos pos): pos(std::move(pos)) {}
    UnterminatedComment::UnterminatedComment(std::vector<pos::Pos> poss): poss(std::move(poss)) {}
    InvalidIntegerLiteral::InvalidIntegerLiteral(std::exception &error, pos::Range pos): error(error), pos(std::move(pos)) {}
    NoExpressionAfterOperator::NoExpressionAfterOperator(pos::Range pos): pos(std::move(pos)) {}
    NoClosingParenthesis::NoClosingParenthesis(pos::Range pos): pos(std::move(pos)) {}
    EmptyParenthesis::EmptyParenthesis(pos::Range pos): pos(std::move(pos)) {}

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
    void NoExpressionAfterOperator::eprint(const std::vector<std::string> &log) const {
        std::cerr << "an expression expected after an operator at " << pos << std::endl;
        pos.eprint(log);
    }
    void NoClosingParenthesis::eprint(const std::vector<std::string> &log) const {
        std::cerr << "no closing parenthesis (opened at " << pos << ")" << std::endl;
        pos.eprint(log);
    }
    void EmptyParenthesis::eprint(const std::vector<std::string> &log) const {
        std::cerr << "empty parenthesis (opened at " << pos << ")" << std::endl;
        pos.eprint(log);
    }
}
