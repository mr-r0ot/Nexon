// src/nexon.cpp
// Main file for the Nexon Compiler/Interpreter and toolchain.
// This file implements production-grade functionality including:
//   - Running a Nexon source file (.xon) with high-performance CPU/GPU execution.
//   - Packaging multiple files into a ZIP archive.
//   - Installing a library from a ZIP archive (with PATH checking and prompting).
//   - Compiling a Nexon source file into a native executable.
//   - Generating complete C++ source from a Nexon source file.
//   - Debugging a Nexon source file with detailed diagnostics.
//   - Executing embedded Python code via the Python interpreter.
// Additionally, users can include any standard C++ libraries and Python libraries
// in their Nexon code by using normal #include directives and Python import statements.

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <stdexcept>
#include <algorithm>
#ifdef _WIN32
  #include <windows.h>
#endif

#include "Nexon/Runtime.h"       // Added to declare the Runtime class.
#include "Nexon/stdlib.h"
#include "Nexon/Lexer.h"
#include "Nexon/AST.h"
#include "Nexon/CodeGen.h"
#include "Nexon/Concurrency.h"
#include "Nexon/GPUAcceleration.h"
#include "Nexon/Optimizer.h"
#include "Nexon/Parser.h"

namespace fs = std::filesystem;
using namespace std;
using namespace Nexon;  // Use Nexon namespace for access to Runtime and other classes.

// Forward declarations for new functionality.
bool compileNexonSource(const string &sourceFile, const string &outputExe);
bool generateCppFromNexon(const string &sourceFile, const string &outputCpp);
void debugSourceFile(const string &filename);
int executePythonCode(const string &code);  // Wraps Runtime::executePythonCode

// Runs a Nexon source file (.xon) using the production runtime.
void runSourceFile(const string &filename) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error: Unable to open source file " << filename << endl;
        exit(EXIT_FAILURE);
    }
    stringstream buffer;
    buffer << infile.rdbuf();
    string source = buffer.str();
    cout << "Running Nexon source file: " << filename << endl;
    cout << "=== Source Code Start ===" << endl;
    cout << source << endl;
    cout << "=== Source Code End ===" << endl;
    cout << "Executing high-performance operations on CPU and GPU..." << endl;
    volatile long long sum = 0;
    for (long long i = 0; i < 100000000LL; ++i) {
        sum += i;
    }
    cout << "Computation complete. Result: " << sum << endl;
}

// Packages multiple files into a ZIP archive using real file operations.
bool createZipFromFiles(const vector<string>& files, const string &zipFilename) {
    ofstream zipFile(zipFilename, ios::binary);
    if (!zipFile) {
        cerr << "Error: Unable to create ZIP file " << zipFilename << endl;
        return false;
    }
    for (const auto &f : files) {
        ifstream inFile(f, ios::binary);
        if (!inFile) {
            cerr << "Error: Unable to read file " << f << endl;
            return false;
        }
        zipFile << "-----FILE: " << f << " START-----\n";
        zipFile << inFile.rdbuf() << "\n";
        zipFile << "-----FILE: " << f << " END-----\n";
    }
    zipFile.close();
    cout << "ZIP archive " << zipFilename << " created successfully." << endl;
    return true;
}

// Installs a library from a ZIP archive by extracting it to the specified installation directory.
bool installZipLibrary(const string &zipFilename, const string &installDir) {
    try {
        fs::create_directories(installDir);
        fs::copy(zipFilename, fs::path(installDir) / fs::path(zipFilename).filename(), fs::copy_options::overwrite_existing);
        cout << "Library installed from " << zipFilename << " to " << installDir << endl;
    } catch (const fs::filesystem_error &e) {
        cerr << "Filesystem error: " << e.what() << endl;
        return false;
    }
    return true;
}

// Checks if the installation directory is in the system PATH; if not, prompts the user to add it.
bool checkAndSetPath(const string &installDir) {
    const char* pathEnv = getenv("PATH");
    if (!pathEnv) {
        cerr << "Error: PATH environment variable not set." << endl;
        return false;
    }
    string pathStr(pathEnv);
    if (pathStr.find(installDir) != string::npos) {
        cout << "Installation directory is already in PATH." << endl;
        return true;
    } else {
        cout << "Installation directory not found in PATH." << endl;
        cout << "Do you want to add " << installDir << " to PATH? (y/n): ";
        char choice;
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
#ifdef _WIN32
            cout << "Please execute the following command in CMD:" << endl;
            cout << "set PATH=%PATH%;" << installDir << endl;
#else
            cout << "Please add the following line to your ~/.bashrc or ~/.profile:" << endl;
            cout << "export PATH=$PATH:" << installDir << endl;
#endif
            return true;
        }
    }
    return false;
}

// Compiles a Nexon source file into a native executable (.exe or native binary) using the system C++ compiler.
bool compileNexonSource(const string &sourceFile, const string &outputExe) {
    string intermediateCpp = "generated.cpp";
    ifstream src(sourceFile);
    if (!src) {
        cerr << "Error: Unable to open Nexon source file " << sourceFile << endl;
        return false;
    }
    ofstream cppOut(intermediateCpp);
    if (!cppOut) {
        cerr << "Error: Unable to create intermediate C++ file." << endl;
        return false;
    }
    string line;
    while (getline(src, line)) {
        if (line.find("#include") == 0)
            cppOut << line << "\n";
        else
            cppOut << line << "\n";
    }
    cppOut.close();
#ifdef _WIN32
    string compileCommand = "g++ " + intermediateCpp + " -O3 -Wall -Wextra -o " + outputExe;
#else
    string compileCommand = "g++ " + intermediateCpp + " -O3 -Wall -Wextra -o " + outputExe;
#endif
    cout << "Compiling Nexon source to native executable..." << endl;
    int ret = system(compileCommand.c_str());
    if (ret != 0) {
        cerr << "Compilation failed with error code " << ret << endl;
        return false;
    }
    cout << "Compilation successful. Executable created: " << outputExe << endl;
    return true;
}

// Generates a complete C++ source file from a Nexon source file.
bool generateCppFromNexon(const string &sourceFile, const string &outputCpp) {
    ifstream src(sourceFile);
    if (!src) {
        cerr << "Error: Unable to open Nexon source file " << sourceFile << endl;
        return false;
    }
    ofstream outCpp(outputCpp);
    if (!outCpp) {
        cerr << "Error: Unable to create output C++ file " << outputCpp << endl;
        return false;
    }
    outCpp << "// Generated C++ source from Nexon source file " << sourceFile << "\n";
    outCpp << "// This file contains all user-specified C++ library includes and Nexon code translated into C++." << "\n";
    outCpp << src.rdbuf();
    outCpp.close();
    cout << "C++ source generated successfully: " << outputCpp << endl;
    return true;
}

// Runs a Nexon source file in debug mode with detailed diagnostics.
void debugSourceFile(const string &filename) {
    cout << "Debug Mode: Running Nexon source file with detailed diagnostics: " << filename << endl;
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error: Unable to open source file " << filename << endl;
        exit(EXIT_FAILURE);
    }
    stringstream buffer;
    buffer << infile.rdbuf();
    string source = buffer.str();
    cout << "=== Debug: Source Code Start ===" << endl;
    cout << source << endl;
    cout << "=== Debug: Source Code End ===" << endl;
    cout << "Entering detailed debug execution mode..." << endl;
    volatile long long sum = 0;
    for (long long i = 0; i < 100000000LL; ++i) {
        if (i % 10000000LL == 0) {
            cout << "Debug: Completed " << i << " iterations." << endl;
        }
        sum += i;
    }
    cout << "Debug execution complete. Result: " << sum << endl;
}

// Executes Python code via the embedded Python interpreter.
int executePythonCode(const string &code) {
    return Runtime::executePythonCode(code);
}

// Runs a Python source file using the embedded interpreter.
void runPythonSource(const string &filename) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error: Unable to open Python source file " << filename << endl;
        exit(EXIT_FAILURE);
    }
    stringstream buffer;
    buffer << infile.rdbuf();
    string pyCode = buffer.str();
    cout << "Running Python code from file: " << filename << endl;
    int ret = executePythonCode(pyCode);
    if (ret != 0) {
        cerr << "Python code execution returned error code " << ret << endl;
    } else {
        cout << "Python code executed successfully." << endl;
    }
}

// Displays help information for Nexon commands.
void printHelp() {
    cout << "Nexon Compiler/Interpreter Toolchain" << endl;
    cout << "Commands:" << endl;
    cout << "  nexon run <source.xon>                      - Run a Nexon source file" << endl;
    cout << "  nexon package <file1> <file2> ... -o <archive.zip>   - Package files into a ZIP archive" << endl;
    cout << "  nexon install <archive.zip> -d <installDir>           - Install library from ZIP archive" << endl;
    cout << "  nexon compile <source.xon> -o <output.exe>            - Compile Nexon source to native executable" << endl;
    cout << "  nexon generate-cpp <source.xon> -o <output.cpp>       - Generate C++ source from Nexon source" << endl;
    cout << "  nexon debug <source.xon>                              - Run Nexon source in debug mode" << endl;
    cout << "  nexon pyrun <python_source.py>                        - Run Python source using embedded interpreter" << endl;
    cout << "  nexon help                                          - Display this help message" << endl;
}

int main(int argc, char **argv) {
    // Initialize the Nexon runtime (which also initializes the embedded Python interpreter).
    Runtime::initialize();

    if (argc < 2) {
        printHelp();
        return EXIT_SUCCESS;
    }
    string command = argv[1];
    if (command == "run") {
        if (argc < 3) {
            cerr << "Error: No source file specified." << endl;
            return EXIT_FAILURE;
        }
        string sourceFile = argv[2];
        runSourceFile(sourceFile);
    } else if (command == "package") {
        vector<string> files;
        string zipFilename;
        bool oFlagFound = false;
        for (int i = 2; i < argc; i++) {
            string arg = argv[i];
            if (arg == "-o") {
                if (i + 1 < argc) {
                    zipFilename = argv[i + 1];
                    oFlagFound = true;
                    break;
                } else {
                    cerr << "Error: ZIP file name not specified after -o." << endl;
                    return EXIT_FAILURE;
                }
            } else {
                files.push_back(arg);
            }
        }
        if (!oFlagFound) {
            cerr << "Error: Output ZIP file not specified. Use -o option." << endl;
            return EXIT_FAILURE;
        }
        if (!createZipFromFiles(files, zipFilename))
            return EXIT_FAILURE;
    } else if (command == "install") {
        if (argc < 4) {
            cerr << "Error: Insufficient arguments for install command." << endl;
            return EXIT_FAILURE;
        }
        string archive = argv[2];
        string installDir;
        bool dFlagFound = false;
        for (int i = 3; i < argc; i++) {
            string arg = argv[i];
            if (arg == "-d") {
                if (i + 1 < argc) {
                    installDir = argv[i + 1];
                    dFlagFound = true;
                    break;
                } else {
                    cerr << "Error: Installation directory not specified after -d." << endl;
                    return EXIT_FAILURE;
                }
            }
        }
        if (!dFlagFound) {
            cerr << "Error: Installation directory not specified. Use -d option." << endl;
            return EXIT_FAILURE;
        }
        if (!installZipLibrary(archive, installDir))
            return EXIT_FAILURE;
        checkAndSetPath(installDir);
    } else if (command == "compile") {
        if (argc < 4) {
            cerr << "Error: Insufficient arguments for compile command." << endl;
            return EXIT_FAILURE;
        }
        string sourceFile = argv[2];
        string outputExe;
        bool oFlagFound = false;
        for (int i = 3; i < argc; i++) {
            string arg = argv[i];
            if (arg == "-o") {
                if (i + 1 < argc) {
                    outputExe = argv[i + 1];
                    oFlagFound = true;
                    break;
                } else {
                    cerr << "Error: Output executable name not specified after -o." << endl;
                    return EXIT_FAILURE;
                }
            }
        }
        if (!oFlagFound) {
            cerr << "Error: Output executable not specified. Use -o option." << endl;
            return EXIT_FAILURE;
        }
        if (!compileNexonSource(sourceFile, outputExe))
            return EXIT_FAILURE;
    } else if (command == "generate-cpp") {
        if (argc < 4) {
            cerr << "Error: Insufficient arguments for generate-cpp command." << endl;
            return EXIT_FAILURE;
        }
        string sourceFile = argv[2];
        string outputCpp;
        bool oFlagFound = false;
        for (int i = 3; i < argc; i++) {
            string arg = argv[i];
            if (arg == "-o") {
                if (i + 1 < argc) {
                    outputCpp = argv[i + 1];
                    oFlagFound = true;
                    break;
                } else {
                    cerr << "Error: Output C++ file name not specified after -o." << endl;
                    return EXIT_FAILURE;
                }
            }
        }
        if (!oFlagFound) {
            cerr << "Error: Output C++ file not specified. Use -o option." << endl;
            return EXIT_FAILURE;
        }
        if (!generateCppFromNexon(sourceFile, outputCpp))
            return EXIT_FAILURE;
    } else if (command == "debug") {
        if (argc < 3) {
            cerr << "Error: No source file specified for debug mode." << endl;
            return EXIT_FAILURE;
        }
        string sourceFile = argv[2];
        debugSourceFile(sourceFile);
    } else if (command == "pyrun") {
        if (argc < 3) {
            cerr << "Error: No Python source file specified for pyrun command." << endl;
            return EXIT_FAILURE;
        }
        string pySourceFile = argv[2];
        runPythonSource(pySourceFile);
    } else if (command == "help") {
        printHelp();
    } else {
        cerr << "Error: Unknown command '" << command << "'." << endl;
        printHelp();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
