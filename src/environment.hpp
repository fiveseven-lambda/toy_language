#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <memory>
#include <map>
#include <string>

#include "context.hpp"
#include "type.hpp"
#include "sentence.hpp"

#include "llvm/ExecutionEngine/Orc/LLJIT.h"

namespace environment {
    class Environment {
        context::Context context;
        std::unique_ptr<llvm::orc::LLJIT> jit;
        std::map<std::string, std::pair<std::shared_ptr<type::Type>, std::size_t>> global_variables;
        std::size_t counter;
    public:
        Environment();
        int run(std::unique_ptr<sentence::Sentence> &);
    };
}

#endif
