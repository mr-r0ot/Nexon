#include "Nexon/Concurrency.h"
#include <thread>
#include <vector>
#include <functional>
#include <iostream>
#include <chrono>

namespace Nexon {

void Concurrency::parallelFor(size_t start, size_t end, const std::function<void(size_t)> &func) {
    unsigned numThreads = std::thread::hardware_concurrency();
    if(numThreads == 0) numThreads = 4;
    size_t total = end - start;
    size_t chunk = (total + numThreads - 1) / numThreads;
    std::vector<std::thread> threads;
    for (unsigned t = 0; t < numThreads; ++t) {
        size_t chunkStart = start + t * chunk;
        size_t chunkEnd = std::min(chunkStart + chunk, end);
        threads.emplace_back([=]() {
            for (size_t i = chunkStart; i < chunkEnd; ++i)
                func(i);
        });
    }
    for (auto &th : threads)
        th.join();
}

void benchmarkParallelFor() {
    auto startTime = std::chrono::high_resolution_clock::now();
    parallelFor(0, 1000000, [](size_t i) {
        volatile double x = i * 0.001;
    });
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;
    std::cout << "Parallel for benchmark: " << elapsed.count() << " seconds." << std::endl;
}

void extraConcurrencyRoutine() {
    for (size_t i = 0; i < 100; ++i) {
        std::cout << "Extra concurrency routine iteration " << i << std::endl;
    }
}

void additionalConcurrencyBenchmark() {
    for (size_t i = 0; i < 50; ++i) {
        std::cout << "Additional concurrency work " << i << std::endl;
    }
}
}
