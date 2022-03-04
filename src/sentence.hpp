/**
 * @file sentence.hpp
 * @brief 文を定義する
 */
#ifndef SENTENCE_HPP
#define SENTENCE_HPP

#include "type.hpp"
#include "expression.hpp"
#include "environment.hpp"
#include "value.hpp"

/**
 * @brief 文を定義する．
 * @code
 * <Sentence> ::= <Expression>? `;`
 *              | <Identifier> `:` <Type>? ( `=` <Expression> )? `;`
 *              | `{` <Sentence>* `}`
 *              | `if` `(` <Expression> `)` <Sentence> ( `else` <Sentence> )?
 *              | `while` `(` <Expression> `)` <Sentence>
 * @endcode
 */
namespace sentence {
    /**
     * @brief 全ての文の基底クラス．
     */
    class Sentence {
        virtual void compile_inner(Environment &, std::unordered_map<std::string, value::Value> &) = 0;
    public:
        //! ソースコード中の位置．
        pos::Range pos;
        virtual ~Sentence();
        llvm::orc::ThreadSafeModule compile(Environment &);
        //! デバッグ出力用の関数．いずれ消す．
        virtual void debug_print(int = 0) const = 0;
    };

    /**
     * @brief 単一の式に `;` が付いた文
     */
    class Expression : public Sentence {
        std::unique_ptr<expression::Expression> expression;
        void compile_inner(Environment &, std::unordered_map<std::string, value::Value> &) override;
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
        void compile_inner(Environment &, std::unordered_map<std::string, value::Value> &) override;
        void debug_print(int) const override;
    public:
        Declaration(std::string, std::unique_ptr<type::Type>, std::unique_ptr<expression::Expression>);
    };

    /**
     * @brief ブロック
     */
    class Block : public Sentence {
        std::vector<std::unique_ptr<Sentence>> sentences;
        void compile_inner(Environment &, std::unordered_map<std::string, value::Value> &) override;
        void debug_print(int) const override;
    public:
        Block(std::vector<std::unique_ptr<Sentence>>);
    };

    /**
     * @brief if 文
     */
    class If : public Sentence {
        std::unique_ptr<expression::Expression> condition;
        std::unique_ptr<Sentence> if_clause, else_clause;
        void compile_inner(Environment &, std::unordered_map<std::string, value::Value> &) override;
        void debug_print(int) const override;
    public:
        If(std::unique_ptr<expression::Expression>, std::unique_ptr<Sentence>, std::unique_ptr<Sentence>);
    };

    /**
     * @brief while 文
     */
    class While : public Sentence {
        std::unique_ptr<expression::Expression> condition;
        std::unique_ptr<Sentence> sentence;
        void compile_inner(Environment &, std::unordered_map<std::string, value::Value> &) override;
        void debug_print(int) const override;
    public:
        While(std::unique_ptr<expression::Expression>, std::unique_ptr<Sentence>);
    };
}

#endif
