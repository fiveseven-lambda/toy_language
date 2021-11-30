#ifndef SENTENCE_HPP
#define SENTENCE_HPP

#include <memory>
#include <string>
#include <map>

#include "pos.hpp"
#include "expression.hpp"
#include "type.hpp"
#include "context.hpp"

#include "llvm/IR/Module.h"

namespace sentence {
    class Sentence {
    public:
        pos::Range pos;
        virtual ~Sentence();
        virtual void translate(context::Context &, std::unique_ptr<llvm::Module> &, std::map<std::string, std::pair<std::shared_ptr<type::Type>, std::size_t>> &) = 0;
        virtual void debug_print(int = 0) const = 0;
    };

    class Expression : public Sentence {
        std::unique_ptr<expression::Expression> expression;
    public:
        Expression(std::unique_ptr<expression::Expression>);
        void translate(context::Context &, std::unique_ptr<llvm::Module> &, std::map<std::string, std::pair<std::shared_ptr<type::Type>, std::size_t>> &) override;
        void debug_print(int) const override;
    };

    class Declaration : public Sentence {
        std::string name;
        std::unique_ptr<type::Type> type;
        std::unique_ptr<expression::Expression> expression;
    public:
        Declaration(std::string, std::unique_ptr<type::Type>, std::unique_ptr<expression::Expression>);
        void translate(context::Context &, std::unique_ptr<llvm::Module> &, std::map<std::string, std::pair<std::shared_ptr<type::Type>, std::size_t>> &) override;
        void debug_print(int) const override;
    };
}

#endif
