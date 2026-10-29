cmake_minimum_required(VERSION 4.0)
project(GBLatorCPP)

set(CMAKE_CXX_STANDARD 20)

add_executable(GBLatorCPP src/main.cpp
        src/cartridge.cpp
        src/cartridge.h)
