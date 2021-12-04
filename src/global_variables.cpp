#include "global_variables.hpp"

#include <sstream>

namespace global_variables {
    GlobalVariables::GlobalVariables(): counter(0) {}
    std::map<std::string, std::pair<std::shared_ptr<type::Type>, llvm::Value *>> GlobalVariables::variables(context::Context &context, std::unique_ptr<llvm::Module> &module){
        std::map<std::string, std::pair<std::shared_ptr<type::Type>, llvm::Value *>> ret;
        for(auto &[name, variable]: map){
            std::stringstream number;
            number << "v" << variable.second;
            llvm::Value *ptr = new llvm::GlobalVariable(
                *module,
                variable.first->llvm_type(context),
                false,
                llvm::GlobalValue::ExternalLinkage,
                nullptr,
                number.str()
            );
            ret.emplace(name, std::make_pair(variable.first, ptr));
        }
        return ret;
    }
    std::size_t GlobalVariables::insert(std::string &name, std::shared_ptr<type::Type> type){
        map[name] = std::make_pair(type, counter);
        return counter++;
    }
}
