/**
 * @file sentence.hpp
 * @brief 文を定義する
 */
#ifndef SENTENCE_HPP
#define SENTENCE_HPP

#include "type.hpp"
#include "expression.hpp"

/**
 * @brief 文を定義する．
 * @code
 * <Sentence> ::= <Expression> `;`
 *              | <Identifier> `:` <Type>? ( `=` <Expression> )? `;`
 * @endcode
 */
namespace sentence {
    /**
     * @brief 全ての文の基底クラス．
     */
    class Sentence {
    public:
        //! ソースコード中の位置．
        pos::Range pos;
        virtual ~Sentence();
        //! デバッグ出力用の関数．いずれ消す．
        virtual void debug_print(int = 0) const = 0;
    };

    /**
     * @brief 単一の式に `;` が付いた文
     */
    class Expression : public Sentence {
        std::unique_ptr<expression::Expression> expression;
        void debug_print(int) const override;
    public:
        Expression(std::unique_ptr<expression::Expression>);
    };

    /**
     * @brief 変数宣言
     */
    class Declaration : public Sentence {
        std::string name;
        std::unique_ptr<type::Type> type;
        std::unique_ptr<expression::Expression> expression;
        void debug_print(int) const override;
    public:
        Declaration(std::string, std::unique_ptr<type::Type>, std::unique_ptr<expression::Expression>);
    };
}

#endif
