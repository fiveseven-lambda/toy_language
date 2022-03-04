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
    /**
     * @brief コンストラクタ
     * @param pos 2 項演算子のあった場所
     */
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
    /**
     * @brief コンストラクタ
     * @param pos 式があればその位置
     * @param colon コロンの位置
     */
    NoIdentifierBeforeColon::NoIdentifierBeforeColon(std::optional<pos::Range> pos, pos::Range colon):
        pos(std::move(pos)), colon(std::move(colon)) {}
    /**
     * @brief コンストラクタ
     * @param pos トークンがあればその位置（EOF なら `std::nullopt`）
     * @param declaration 宣言の位置
     */
    NoSemicolonAfterDeclaration::NoSemicolonAfterDeclaration(std::optional<pos::Range> pos, pos::Range declaration):
        pos(std::move(pos)), declaration(std::move(declaration)) {}
    /**
     * @brief コンストラクタ
     * @param pos トークンがあればその位置（EOF なら `std::nullopt`）
     * @param expression 式の位置
     */
    NoSemicolonAfterExpression::NoSemicolonAfterExpression(std::optional<pos::Range> pos, pos::Range expression):
        pos(std::move(pos)), expression(std::move(expression)) {}
    /**
     * @brief コンストラクタ
     * @param pos トークンの位置
     */
    UnexpectedTokenAtSentence::UnexpectedTokenAtSentence(pos::Range pos): pos(std::move(pos)) {}
    /**
     * @brief コンストラクタ
     * @param pos 開き括弧の位置
     */
    NoClosingBrace::NoClosingBrace(pos::Range pos): pos(std::move(pos)) {}
    /**
     * @brief コンストラクタ
     * @param pos トークンがあればその位置（EOF なら `std::nullopt`）
     * @param keyword キーワードの位置
     */
    NoParenthesisAfterKeyword::NoParenthesisAfterKeyword(std::optional<pos::Range> pos, pos::Range keyword):
        pos(std::move(pos)), keyword(std::move(keyword)) {}
    /**
     * @brief コンストラクタ
     * @param open 開き括弧の位置
     * @param close 閉じ括弧の位置
     */
    EmptyCondition::EmptyCondition(pos::Range open, pos::Range close):
        open(std::move(open)),
        close(std::move(close)) {}
    /**
     * @brief コンストラクタ
     * @param pos 制御文の位置
     */
    UnexpectedEOFInControlStatement::UnexpectedEOFInControlStatement(pos::Range pos): pos(std::move(pos)) {}
    /**
     * @brief コンストラクタ
     * @param pos 変数の位置
     */
    UndefinedVariable::UndefinedVariable(pos::Range pos): pos(std::move(pos)) {}

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
    void NoIdentifierBeforeColon::eprint(const std::vector<std::string> &log) const {
        std::cerr << "no identifier";
        if(pos) std::cerr << " at " << pos.value();
        std::cerr << " before colon" << std::endl;
        if(pos) pos.value().eprint(log);
        std::cerr << "note: colon at " << colon << std::endl;
        colon.eprint(log);
    }
    void NoSemicolonAfterDeclaration::eprint(const std::vector<std::string> &log) const {
        std::cerr << "no semicolon";
        if(pos) std::cerr << " at " << pos.value();
        std::cerr << " after declaration" << std::endl;
        if(pos) pos.value().eprint(log);
        std::cerr << "note: declaration at " << declaration << std::endl;
        declaration.eprint(log);
    }
    void NoSemicolonAfterExpression::eprint(const std::vector<std::string> &log) const {
        std::cerr << "no semicolon";
        if(pos) std::cerr << " at " << pos.value();
        std::cerr << " after expression" << std::endl;
        if(pos) pos.value().eprint(log);
        std::cerr << "note: expression at " << expression << std::endl;
        expression.eprint(log);
    }
    void UnexpectedTokenAtSentence::eprint(const std::vector<std::string> &log) const {
        std::cerr << "unexpected token at " << pos << " (expected sentence)" << std::endl;
        pos.eprint(log);
    }
    void NoClosingBrace::eprint(const std::vector<std::string> &log) const {
        std::cerr << "no closing brace (opened at " << pos << ")" << std::endl;
        pos.eprint(log);
    }
    void NoParenthesisAfterKeyword::eprint(const std::vector<std::string> &log) const {
        std::cerr << "opening parenthesis expected";
        if(pos){
            std::cerr << " at " << pos.value() << std::endl;
            pos.value().eprint(log);
        }else{
            std::cerr << std::endl;
        }
        std::cerr << "note: keyword at " << keyword << std::endl;
        keyword.eprint(log);
    }
    void EmptyCondition::eprint(const std::vector<std::string> &log) const {
        std::cerr << "empty parenthesis (opened at " << open << ")" << std::endl;
        open.eprint(log);
        std::cerr << "closed at " << close << ")" << std::endl;
        close.eprint(log);
    }
    void UnexpectedEOFInControlStatement::eprint(const std::vector<std::string> &log) const {
        std::cerr << "unexpected EOF in control statement at " << pos << std::endl;
        pos.eprint(log);
    }
    void UndefinedVariable::eprint(const std::vector<std::string> &log) const {
        std::cerr << "undefined variable at " << pos << std::endl;
        pos.eprint(log);
    }
}
