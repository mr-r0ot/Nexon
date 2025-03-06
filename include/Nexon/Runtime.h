#ifndef NEXON_RUNTIME_H
#define NEXON_RUNTIME_H

#include <iostream>
#include <string>

namespace Nexon {

    // Runtime initializes and shuts down the Nexon runtime environment.
    // It now also initializes the embedded Python interpreter so that
    // users can import and use any Python library in their Nexon code.
    class Runtime {
    public:
        static void initialize();
        static void shutdown();
        // Initialize the embedded Python interpreter.
        static void initializePython();
        // Finalize the Python interpreter.
        static void finalizePython();
        // Execute a Python string.
        static int executePythonCode(const std::string &code);
    };

}
#endif // NEXON_RUNTIME_H
