#pragma once
#ifndef GBLATORCPP_BUS_H
#define GBLATORCPP_BUS_H
#include <cstdint>
#include <array>
#include <iostream>

class Cartridge;

class Bus {
public:
    Bus(const Cartridge& cart);
    uint8_t read(uint16_t addr) const {
        if (addr == 0xFF44) return 0x90;
        return mem[addr];
    }
    void write(uint16_t addr, uint8_t data) {
        mem[addr] = data;
        if (addr == 0xFF02 && data == 0x81) {   // serial transfer start
            std::cout << static_cast<char>(mem[0xFF01]) << std::flush;
        }
    }

private:
    std::array<uint8_t, 0x10000> mem {};
};


#endif //GBLATORCPP_BUS_H