#ifndef SENTENCE_HPP
#define SENTENCE_HPP

#include <memory>
#include <string>

#include "pos.hpp"
#include "expression.hpp"
#include "type.hpp"

namespace sentence {
    class Sentence {
    public:
        pos::Range pos;
        virtual ~Sentence();
        virtual void debug_print(int = 0) const = 0;
    };

    class Expression : public Sentence {
        std::unique_ptr<expression::Expression> expression;
    public:
        Expression(std::unique_ptr<expression::Expression>);
        void debug_print(int) const override;
    };

    class Declaration : public Sentence {
        std::string name;
        std::unique_ptr<type::Type> type;
        std::unique_ptr<expression::Expression> expression;
    public:
        Declaration(std::string, std::unique_ptr<type::Type>, std::unique_ptr<expression::Expression>);
        void debug_print(int) const override;
    };
}

#endif
