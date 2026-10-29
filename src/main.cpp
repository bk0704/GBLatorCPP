#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include "cartridge.h"

using namespace std;
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: GBLatorCPP <rom file>" << endl;
        return 1;
    }

    Cartridge cartridge;
    if (!cartridge.load(argv[1])) {
        std::cerr << "Can't open ROM file: " << argv[1] << "\n";
        return 1;
    }

    std::cout << "Read " << cartridge.size() << " bytes\n";
    std::cout << "Title: " << cartridge.title() << "\n";
    return 0;
}