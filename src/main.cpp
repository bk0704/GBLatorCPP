#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include "cartridge.h"
#include "bus.h"

using namespace std;
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: GBLatorCPP <rom file>" << endl;
        return 1;
    }

    Cartridge cartridge;
    if (!cartridge.load(argv[1])) {
        cerr << "Can't open ROM file: " << argv[1] << "\n";
        return 1;
    }

    cout << "Read " << cartridge.size() << " bytes\n";
    cout << "Title: " << cartridge.title() << "\n";

    Bus bus(cartridge);
    cout << "Bus read at 0x0134: " << bus.read(0x0134) << "\n";

    bus.write(0xC000, 0x42);
    uint8_t result = bus.read(0xC000);
    cout << hex << static_cast<int>(result) << "\n";

    return 0;
}