#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <memory>
#include <map>
#include <string>

#include "context.hpp"
#include "type.hpp"
#include "sentence.hpp"
#include "global_variables.hpp"

#include "llvm/ExecutionEngine/Orc/LLJIT.h"

namespace environment {
    class Environment {
        context::Context context;
        std::unique_ptr<llvm::orc::LLJIT> jit;
        global_variables::GlobalVariables global_variables;
        std::size_t counter;
    public:
        Environment();
        int run(std::unique_ptr<sentence::Sentence> &);
    };
}

#endif
