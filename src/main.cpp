#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include "cartridge.h"
#include "bus.h"
#include "CPU/cpu.h"

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
    CPU cpu(bus);
    cpu.reset();
    FILE* log = fopen("doctor.log", "w");
    while (true) {
        cpu.log_state(log);
        cpu.step();
    }

    return 0;
}