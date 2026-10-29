#include "cpu.h"
#include <iostream>

using namespace std;
int CPU::execute(uint8_t op) {
    uint8_t x = op >> 6;
    uint8_t y = (op >> 3) & 7;
    uint8_t z = op & 7;

    uint8_t p = y >> 1;
    uint8_t q = y & 1;

    switch (op) {
        // NOP
        case 0x00:
            return 4;
        // JP a16
        case 0xC3:
            PC = fetch16();
            return 16;
        // DI
        case 0xF3:
            IME = false;
            return 4;
        // EI
        case 0xFB:
            ei_pending = true;
            return 4;
        // HALT
        case 0x76:
            halted = true;
            return 4;
    }

    // LD r, r'
    if (x == 1) {
        set_r8(y, r8(z));
        return 4;
    }


    cerr << "Unimplemented opcode 0x" << hex << int(op) << " at 0x" << (PC - 1) << "\n";
    abort();
}