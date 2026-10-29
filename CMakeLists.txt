cmake_minimum_required(VERSION 4.0)
project(GBLatorCPP)

set(CMAKE_CXX_STANDARD 23)

add_executable(GBLatorCPP src/main.cpp
        src/cartridge.cpp
        src/cartridge.h
        src/bus.cpp
        src/bus.h
        src/CPU/cpu.cpp
        src/CPU/cpu.h
        src/CPU/opcodes.cpp
        src/CPU/disasm.cpp
        src/CPU/disasm.h)
