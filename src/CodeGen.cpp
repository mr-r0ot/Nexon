#include "Nexon/CodeGen.h"
#include <iostream>
#include <chrono>

using namespace llvm;
using namespace Nexon;

std::unique_ptr<LLVMContext> CodeGen::GlobalContext = std::make_unique<LLVMContext>();
std::unique_ptr<Module> CodeGen::ModuleInstance = std::make_unique<Module>("Nexon Module", *GlobalContext);
std::unique_ptr<IRBuilder<>> CodeGen::IRBuilderInstance = std::make_unique<IRBuilder<>>(*GlobalContext);
std::map<std::string, llvm::Value*> CodeGen::NamedValues;

LLVMContext &CodeGen::getGlobalContext() {
    return *GlobalContext;
}

IRBuilder<>* CodeGen::Builder() {
    return IRBuilderInstance.get();
}

Module* CodeGen::TheModule() {
    return ModuleInstance.get();
}

llvm::Value* CodeGen::getNamedValue(const std::string &Name) {
    return NamedValues[Name];
}

void CodeGen::setNamedValue(const std::string &Name, llvm::Value* V) {
    NamedValues[Name] = V;
}

void CodeGen::clearNamedValues() {
    NamedValues.clear();
}

void dumpModule() {
    ModuleInstance->print(llvm::errs(), nullptr);
}

void printGlobalVariables() {
    for (auto &GV : ModuleInstance->globals()) {
        llvm::errs() << "Global Variable: " << GV.getName() << "\n";
    }
}

void performDummyWork() {
    volatile int counter = 0;
    for (int i = 0; i < 500; ++i) {
        counter += i;
    }
    std::cout << "Dummy work done, counter: " << counter << "\n";
}

void extraCodeGenRoutine() {
    for (int i = 0; i < 50; ++i) {
        std::cout << "Extra CodeGen iteration " << i << std::endl;
    }
}
