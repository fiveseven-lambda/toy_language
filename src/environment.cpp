#include "environment.hpp"

#include <sstream>

#include "error.hpp"
#include "llvm/IR/Module.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"

namespace environment {
    Environment::Environment(): counter(0) {
        LLVMInitializeNativeTarget();
        jit = std::move(llvm::orc::LLJITBuilder().create().get());
        LLVMInitializeNativeAsmPrinter();
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
        if(auto error = jit->addIRModule(llvm::orc::ThreadSafeModule(std::move(module), context.context))){
            throw error::make<error::LLVMError>(std::move(error));
        }
        if(auto result = jit->lookup(counter_name.str())){
            auto compiled_function = reinterpret_cast<void(*)()>(result.get().getAddress());
            compiled_function();
        }else{
            throw error::make<error::LLVMError>(result.takeError());
        }
        ++counter;
        return 0;
    }
}
