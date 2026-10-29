#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "cartridge.h"
#include "bus.h"
#include "CPU/cpu.h"

using namespace std;
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: GBLatorCPP <rom file> [--trace]" << endl;
        return 1;
    }
    bool trace = argc > 2 && string(argv[2]) == "--trace";

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
    FILE* trace_log = trace ? fopen("trace.log", "w") : nullptr;
    while (true) {
        cpu.log_state(log);
        if (trace_log) cpu.trace_state(trace_log);
        cpu.step();
    }

    return 0;
}