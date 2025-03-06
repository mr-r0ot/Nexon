#ifndef NEXON_GPUACCELERATION_H
#define NEXON_GPUACCELERATION_H

#include <iostream>

namespace Nexon {

    // GPUAcceleration provides production-grade GPU acceleration via CUDA.
    class GPUAcceleration {
    public:
        static void runSampleKernel();
    };

}
#endif // NEXON_GPUACCELERATION_H
