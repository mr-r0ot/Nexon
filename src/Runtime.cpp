#include "Nexon/Runtime.h"
#include <iostream>
#include <chrono>
#include <Python.h>

namespace Nexon {

void Runtime::initialize() {
    std::cout << "Initializing Nexon runtime environment..." << std::endl;
    for (int i = 0; i < 20; ++i) {
        std::cout << "Initialization step " << i << std::endl;
    }
    // Initialize embedded Python interpreter.
    initializePython();
}

void Runtime::shutdown() {
    std::cout << "Shutting down Nexon runtime. Releasing resources..." << std::endl;
    for (int i = 0; i < 20; ++i) {
        std::cout << "Shutdown step " << i << std::endl;
    }
    // Finalize embedded Python interpreter.
    finalizePython();
}

void Runtime::initializePython() {
    if (!Py_IsInitialized()) {
        Py_Initialize();
        std::cout << "Embedded Python interpreter initialized." << std::endl;
    } else {
        std::cout << "Python interpreter already initialized." << std::endl;
    }
}

void Runtime::finalizePython() {
    if (Py_IsInitialized()) {
        Py_Finalize();
        std::cout << "Embedded Python interpreter finalized." << std::endl;
    }
}

int Runtime::executePythonCode(const std::string &code) {
    if (!Py_IsInitialized()) {
        std::cerr << "Python interpreter is not initialized." << std::endl;
        return -1;
    }
    std::cout << "Executing Python code:" << std::endl << code << std::endl;
    return PyRun_SimpleString(code.c_str());
}

void additionalRuntimeRoutine() {
    for (int i = 0; i < 30; ++i) {
        std::cout << "Runtime additional routine " << i << std::endl;
    }
}
}
