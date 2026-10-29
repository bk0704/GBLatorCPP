
#include "cartridge.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <system_error>

using namespace std;
bool Cartridge::load(const std::string &path) {
    error_code ec;
    auto fileSize = filesystem::file_size(path, ec);
    if (ec) {
        return false;
    }

    ifstream file(path, ios::binary);
    if (!file.is_open()) {
        return false;
    }
    rom.resize(fileSize);
    file.read(reinterpret_cast<char *>(&rom[0]), rom.size());

    return file.good();
}

std::string Cartridge::title() const {
    string result;

    for (int addr = 0x0134; addr <= 0x0143; addr++) {
        uint8_t byte = read(addr);
        if (byte == 0) {
            break;
        }
        result.push_back(static_cast<char>(byte));
    }

    return result;
}


