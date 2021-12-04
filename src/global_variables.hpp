#ifndef GLOBAL_VARIABLES_HPP
#define GLOBAL_VARIABLES_HPP

#include <cstddef>
#include <memory>
#include <utility>
#include <unordered_map>
#include <string>
#include "type.hpp"
#include "context.hpp"

namespace global_variables {
    class GlobalVariables {
        std::size_t counter;
        std::unordered_map<std::string, std::pair<std::shared_ptr<type::Type>, std::size_t>> map;
    public:
        GlobalVariables();
        std::map<std::string, std::pair<std::shared_ptr<type::Type>, llvm::Value *>> variables(context::Context &, std::unique_ptr<llvm::Module> &);
        std::size_t insert(std::string &, std::shared_ptr<type::Type>);
    };
}

#endif
