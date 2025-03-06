#ifndef NEXON_CODEGEN_H
#define NEXON_CODEGEN_H

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <map>
#include <string>
#include <memory>

namespace Nexon {

    // CodeGen provides production-grade LLVM-based code generation.
    class CodeGen {
    public:
        static llvm::LLVMContext &getGlobalContext();
        static llvm::IRBuilder<>* Builder();
        static llvm::Module* TheModule();
        static llvm::Value* getNamedValue(const std::string &Name);
        static void setNamedValue(const std::string &Name, llvm::Value* V);
        static void clearNamedValues();
    private:
        static std::unique_ptr<llvm::LLVMContext> GlobalContext;
        static std::unique_ptr<llvm::Module> ModuleInstance;
        static std::unique_ptr<llvm::IRBuilder<>> IRBuilderInstance;
        static std::map<std::string, llvm::Value*> NamedValues;
    };

}
#endif // NEXON_CODEGEN_H
