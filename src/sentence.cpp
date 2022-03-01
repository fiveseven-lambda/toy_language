/**
 * @file sentence.cpp
 */
#include "sentence.hpp"

namespace sentence {
    Sentence::~Sentence() = default;
    //! コンストラクタ
    Expression::Expression(std::unique_ptr<expression::Expression> expression):
        expression(std::move(expression)) {}
    /**
     * @brief コンストラクタ
     * @param name 宣言された変数名
     * @param type 型
     * @param expression 初期化の式
     */
    Declaration::Declaration(
        std::string name,
        std::unique_ptr<type::Type> type,
        std::unique_ptr<expression::Expression> expression
    ):
        name(std::move(name)),
        type(std::move(type)),
        expression(std::move(expression)) {}
    /**
     * @brief コンストラクタ
     * @param sentences 中身
     */
    Block::Block(std::vector<std::unique_ptr<Sentence>> sentences):
        sentences(std::move(sentences)) {}
    /**
     * @brief コンストラクタ
     * @param condition 条件
     * @param if_clause if 節
     * @param else_clause else 節（空なら nullptr）
     */
    If::If(
        std::unique_ptr<expression::Expression> condition,
        std::unique_ptr<Sentence> if_clause,
        std::unique_ptr<Sentence> else_clause
    ):
        condition(std::move(condition)),
        if_clause(std::move(if_clause)),
        else_clause(std::move(else_clause)) {}
    /**
     * @brief コンストラクタ
     * @param condition 条件
     * @param sentence 中身
     */
    While::While(
        std::unique_ptr<expression::Expression> condition,
        std::unique_ptr<Sentence> sentence
    ):
        condition(std::move(condition)),
        sentence(std::move(sentence)) {}

    static constexpr std::string_view INDENT = "    ";
    void Expression::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        if(expression){
            std::cout << pos << ": Expression" << std::endl;
            expression->debug_print(depth + 1);
        }else{
            std::cout << pos << ": Expression (empty)" << std::endl;
        }
    }
    void Declaration::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": Declaration(" << name << ")" << std::endl;
        if(type) type->debug_print(depth + 1);
        if(expression) expression->debug_print(depth + 1);
    }
    void Block::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": Block (" << sentences.size() << " sentences)" << std::endl;
        for(auto &sentence : sentences){
            sentence->debug_print(depth + 1);
        }
    }
    void If::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": If" << std::endl;
        condition->debug_print(depth + 1);
        if_clause->debug_print(depth + 1);
        if(else_clause) else_clause->debug_print(depth + 1);
    }
    void While::debug_print(int depth) const {
        for(int i = 0; i < depth; ++i) std::cout << INDENT;
        std::cout << pos << ": While" << std::endl;
        condition->debug_print(depth + 1);
        sentence->debug_print(depth + 1);
    }
}
