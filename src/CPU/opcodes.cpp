#include "cpu.h"
#include "disasm.h"
#include <iostream>

using namespace std;
int CPU::execute(uint8_t op) {
    uint8_t x = op >> 6;
    uint8_t y = (op >> 3) & 7;
    uint8_t z = op & 7;

    uint8_t p = y >> 1;
    uint8_t q = y & 1;

    uint16_t addr;

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

    if (x == 0) {
        if (z == 1) {
            if (q == 0) {
                // LD rp[p], d16
                set_r16(p, fetch16());
                return 12;
            }
        }
        if (z == 2) {
            // Indirect loading
            if (p < 2) {
                // BC or DE
                addr = r16(p);
            }
            else {
                // HL
                addr = r16(2);
                set_r16(2, p==2 ? addr+1 : addr-1);
            }

            if (q == 0) bus.write(addr, A);
            else A = bus.read(addr);

            return 8;
        }

        if (z == 3) {
            if (q == 0) {
                // INC rp[p]
                set_r16(p, (r16(p)+1));
                return 8;
            }
            // DEC rp[p]
            set_r16(p, r16(p)-1);
            return 8;
        }

        if (z == 4) {
            // INC r8
            uint8_t old = r8(y);
            uint8_t inc_old = old + 1;
            set_r8(y, inc_old);

            setFlag(FLAG_Z, inc_old == 0);
            setFlag(FLAG_N, false);
            setFlag(FLAG_H, (old & 0x0F) == 0x0F);

            return (y == 6) ? 12: 4;

        }

        // LD r8, n8
        if (z == 6) {
            set_r8(y, fetch8());
            return (y == 6) ? 12 : 8;
        }
    }
    // LD r, r'
    if (x == 1) {
        set_r8(y, r8(z));
        return (y == 6 || z == 6) ? 8 : 4;
    }


    int length;
    cerr << "Unimplemented opcode 0x" << hex << int(op) << " (" << disasm(bus, PC - 1, length)
         << ") at 0x" << (PC - 1) << "\n";
    // exit() rather than abort() so the log files get flushed
    exit(1);
}