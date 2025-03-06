cmake_minimum_required(VERSION 3.10)
project(Nexon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Locate LLVM libraries for code generation.
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Locate CUDA for GPU acceleration.
find_package(CUDA)
if(CUDA_FOUND)
  message(STATUS "CUDA found: Enabling GPU acceleration module.")
  include_directories(${CUDA_INCLUDE_DIRS})
  add_definitions(-DHAVE_CUDA)
endif()

# Locate Python3 for embedded Python interoperability.
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
message(STATUS "Found Python3: ${Python3_VERSION}")
include_directories(${Python3_INCLUDE_DIRS})

# Add include directory.
include_directories(${CMAKE_SOURCE_DIR}/include)

# Collect source files.
set(SOURCES
    src/AST.cpp
    src/CodeGen.cpp
    src/Concurrency.cpp
    src/GPUAcceleration.cpp
    src/Lexer.cpp
    src/Optimizer.cpp
    src/Parser.cpp
    src/Runtime.cpp
    src/nexon.cpp
)

# Create the Nexon executable.
add_executable(nexon ${SOURCES})
llvm_map_components_to_libnames(llvm_libs support core irreader native)
target_link_libraries(nexon ${llvm_libs} pthread ${Python3_LIBRARIES})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -DNDEBUG")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
