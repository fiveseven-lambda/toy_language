#include "environment.hpp"

#include <sstream>

#include "llvm/IR/Module.h"

namespace environment {
    Environment::Environment(): counter(0) {
        // jit の初期化
    }
    int Environment::run(std::unique_ptr<sentence::Sentence> &sentence){
        std::stringstream counter_name;
        counter_name << counter;
        std::unique_ptr<llvm::Module> module = std::make_unique<llvm::Module>(counter_name.str(), *context.context.getContext());
        llvm::Function *function = llvm::Function::Create(context.function_type, llvm::GlobalValue::LinkageTypes::ExternalLinkage, counter_name.str(), *module);
        llvm::BasicBlock *basic_block = llvm::BasicBlock::Create(*context.context.getContext(), "entry", function);
        context.builder.SetInsertPoint(basic_block);
        sentence->translate(context, module, global_variables);
        module->print(llvm::errs(), nullptr);
        ++counter;
        return 0;
    }
}
