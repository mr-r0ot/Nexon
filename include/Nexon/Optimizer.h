#ifndef NEXON_OPTIMIZER_H
#define NEXON_OPTIMIZER_H

namespace Nexon {

    // Optimizer runs production-grade optimization passes on the generated LLVM IR.
    class Optimizer {
    public:
        static void runOptimizationPasses();
    };

}
#endif // NEXON_OPTIMIZER_H
