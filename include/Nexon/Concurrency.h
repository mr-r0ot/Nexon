#ifndef NEXON_CONCURRENCY_H
#define NEXON_CONCURRENCY_H

#include <thread>
#include <vector>
#include <functional>
#include <iostream>

namespace Nexon {

    // Concurrency provides production-grade parallel processing using std::thread.
    class Concurrency {
    public:
        static void parallelFor(size_t start, size_t end, const std::function<void(size_t)> &func);
    };

}
#endif // NEXON_CONCURRENCY_H
