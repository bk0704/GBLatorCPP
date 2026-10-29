#pragma once
#ifndef GBLATORCPP_CARTRIDGE_H
#define GBLATORCPP_CARTRIDGE_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

class Cartridge {
public:
    bool load(const std::string& path);
    std::string title() const;
    uint8_t read(uint16_t address) const {
        if (address >= rom.size()) {
            return 0xFF;
        }
        return rom[address];
    }

    std::size_t size() const {
        return rom.size();
    }
private:
    std::vector<uint8_t> rom;
};


#endif //GBLATORCPP_CARTRIDGE_H