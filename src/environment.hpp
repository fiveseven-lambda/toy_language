/**
 * @file environment.hpp
 */
#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <unordered_map>
#include <string>
#include <utility>
#include <memory>

#include "type.hpp"
#include "sentence.hpp"

#include "llvm/ExecutionEngine/Orc/ThreadSafeModule.h"

class Environment {
    llvm::orc::ThreadSafeContext context;
    std::unordered_map<std::string, std::pair<unsigned, std::shared_ptr<type::Type>>> global_variables;
    unsigned module_number;
public:
    Environment();
    llvm::orc::ThreadSafeModule compile(std::unique_ptr<sentence::Sentence>);
    std::string module_name();
};

#endif
