#include "Nexon/Optimizer.h"
#include "Nexon/CodeGen.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Scalar.h"
#include <iostream>

namespace Nexon {

void Optimizer::runOptimizationPasses() {
    llvm::legacy::PassManager passManager;
    passManager.add(llvm::createPromoteMemoryToRegisterPass());
    passManager.add(llvm::createInstructionCombiningPass());
    passManager.add(llvm::createReassociatePass());
    passManager.add(llvm::createGVNPass());
    passManager.add(llvm::createCFGSimplificationPass());
    passManager.run(*CodeGen::TheModule());
    std::cout << "Optimization passes executed." << std::endl;
}

void extraOptimization() {
    std::cout << "Running extra optimization routine..." << std::endl;
    for (int i = 0; i < 50; ++i) {
        volatile int dummy = i * i;
    }
    std::cout << "Extra optimization routine completed." << std::endl;
}

void additionalOptimizerRoutine() {
    for (int i = 0; i < 50; ++i) {
        std::cout << "Additional optimizer iteration " << i << std::endl;
    }
}
}
