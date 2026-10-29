#pragma once
#ifndef GBLATORCPP_BUS_H
#define GBLATORCPP_BUS_H
#include <cstdint>
#include <array>

class Cartridge;

class Bus {
public:
    Bus(const Cartridge& cart);
    uint8_t read(uint16_t addr) const {
        return mem[addr];
    }
    void write(uint16_t addr, uint8_t data) {
        mem[addr] = data;
    }

private:
    std::array<uint8_t, 0x10000> mem {};
};


#endif //GBLATORCPP_BUS_H