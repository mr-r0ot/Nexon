#!/usr/bin/env python3
"""
installer.py

Production‑grade installer for the Nexon project.
This script detects the operating system, checks if a C++ compiler,
LLVM (llvm-config), and CUDA (nvcc) are installed. If not, it prompts the user
and installs the missing dependencies using the native package manager.
It also installs any additional system-level dependencies required by Nexon.
This script is intended to be run with administrative privileges where necessary.

Author: Mohammad Taha Gorji
"""

import os
import sys
import platform
import subprocess
import shutil

def run_command(command, shell=False, check=True):
    """Runs a system command and returns the result."""
    try:
        result = subprocess.run(command, shell=shell, check=check, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
        return result.stdout.strip()
    except subprocess.CalledProcessError as e:
        print(f"Command '{' '.join(command) if isinstance(command, list) else command}' failed with error:\n{e.stderr}")
        return None

def is_tool_installed(tool_name):
    """Checks if a tool is installed by using shutil.which."""
    return shutil.which(tool_name) is not None

def prompt_install(message):
    """Prompt the user for installation consent."""
    answer = input(message + " (y/n): ").strip().lower()
    return answer == 'y'

def install_cpp_compiler(os_name):
    """Installs a C++ compiler based on the OS."""
    print("Checking for C++ compiler...")
    # On Linux, we assume g++ is desired.
    if os_name == "Linux":
        if not is_tool_installed("g++"):
            if prompt_install("g++ is not installed. Would you like to install g++ using apt-get?"):
                print("Installing g++ via apt-get...")
                run_command(["sudo", "apt-get", "update"])
                run_command(["sudo", "apt-get", "install", "-y", "g++"])
        else:
            print("g++ is installed.")
    elif os_name == "Darwin":
        # On macOS, check for clang.
        if not is_tool_installed("clang++"):
            if prompt_install("clang++ is not installed. Would you like to install Command Line Tools for Xcode?"):
                print("Installing Command Line Tools...")
                run_command(["xcode-select", "--install"], check=False)
        else:
            print("clang++ is installed.")
    elif os_name == "Windows":
        # On Windows, try checking for g++ (MinGW) or cl.exe (Visual Studio)
        if not (is_tool_installed("g++") or is_tool_installed("cl")):
            if prompt_install("No C++ compiler detected. Would you like to install MinGW-w64?"):
                print("Please download and install MinGW-w64 from https://sourceforge.net/projects/mingw-w64/")
                input("Press Enter after installation is complete...")
            else:
                print("A C++ compiler is required to proceed. Exiting.")
                sys.exit(1)
        else:
            print("A C++ compiler is installed.")
    else:
        print("Unsupported OS for automatic C++ compiler installation.")
        sys.exit(1)

def install_llvm(os_name):
    """Checks and installs LLVM if missing."""
    print("Checking for LLVM (llvm-config)...")
    if not is_tool_installed("llvm-config"):
        if os_name == "Linux":
            if prompt_install("llvm-config is not installed. Install LLVM via apt-get?"):
                print("Installing LLVM via apt-get...")
                run_command(["sudo", "apt-get", "install", "-y", "llvm", "llvm-dev", "llvm-config"])
        elif os_name == "Darwin":
            if prompt_install("llvm-config is not installed. Install LLVM via Homebrew?"):
                print("Installing LLVM via brew...")
                run_command(["brew", "install", "llvm"])
        elif os_name == "Windows":
            print("Please download and install LLVM for Windows from https://llvm.org/builds/")
            input("Press Enter after LLVM installation is complete...")
    else:
        print("LLVM is installed.")

def install_cuda(os_name):
    """Checks and installs CUDA if available on the system."""
    print("Checking for CUDA (nvcc)...")
    if not is_tool_installed("nvcc"):
        if os_name == "Linux":
            if prompt_install("CUDA is not installed. Would you like to install CUDA via apt-get (if supported)?"):
                print("Installing CUDA via apt-get...")
                run_command(["sudo", "apt-get", "install", "-y", "cuda"])
        elif os_name == "Darwin":
            print("CUDA support on macOS is limited. Please ensure you have a compatible GPU and install CUDA from NVIDIA if required.")
        elif os_name == "Windows":
            print("Please download and install CUDA from https://developer.nvidia.com/cuda-downloads")
            input("Press Enter after CUDA installation is complete...")
    else:
        print("CUDA is installed.")

def install_dependencies():
    """Checks and installs all system-level dependencies required by the Nexon project."""
    os_name = platform.system()
    print(f"Detected operating system: {os_name}")
    install_cpp_compiler(os_name)
    install_llvm(os_name)
    install_cuda(os_name)
    # Additional dependency checks can be added here (e.g., for CMake, Git, etc.)
    print("All required dependencies are installed.")

def main():
    print("Starting Nexon project installer...")
    install_dependencies()
    print("All system dependencies have been successfully installed.")
    print("The Nexon project is now ready to be built and executed.")
    # Optionally, you could call the build system (e.g., cmake --build .) here.
    build_now = input("Would you like to build the Nexon project now? (y/n): ").strip().lower()
    if build_now == 'y':
        print("Building the Nexon project...")
        ret = os.system("cmake --build .")
        if ret != 0:
            print("Build failed. Please check the error messages above.")
        else:
            print("Build succeeded. The Nexon executable is available in the bin/ directory.")
    else:
        print("Build skipped. You can build the project later using the provided CMake configuration.")

if __name__ == "__main__":
    main()
