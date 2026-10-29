
#include "cpu.h"

#include <iostream>

using namespace std;
void CPU::reset() {
    A = 0x01;
    F = 0xB0;
    B = 0x00;
    C = 0x13;
    D = 0x00;
    E = 0xD8;
    H = 0x01;
    L = 0x4D;
    PC = 0x0100;
    SP = 0xFFFE;

    IME = false;
    halted = false;
    stopped = false;
    ei_pending = false;

    cycles_total = 0;
}

uint8_t CPU::r8(int i) {
    switch (i) {
        case 0: return B;
        case 1: return C;
        case 2: return D;
        case 3: return E;
        case 4: return H;
        case 5: return L;
        case 6: return bus.read(HL());
        case 7: return A;
        default:
            cerr << "r8: bad index " << i << "\n";
            abort();
    }
}

void CPU::set_r8(int i, uint8_t v) {
    switch (i) {
        case 0:
            B = v;
            break;
        case 1:
            C = v;
            break;
        case 2:
            D = v;
            break;
        case 3:
            E = v;
            break;
        case 4:
            H = v;
            break;
        case 5:
            L = v;
            break;
        case 6:
            bus.write(HL(), v);
            break;
        case 7:
            A = v;
            break;
        default:
            cerr << "r8: bad index " << i << "\n";
            abort();
    }
}

uint16_t CPU::r16(int i) {
    switch (i) {
        case 0: return BC();
        case 1: return DE();
        case 2: return HL();
        case 3: return SP;
        default:
            cerr << "r16: bad index " << i << "\n";
            abort();
    }
}

void CPU::set_r16(int i, uint16_t v) {
    switch (i) {
        case 0:
            setBC(v);
            break;
        case 1:
            setDE(v);
            break;
        case 2:
            setHL(v);
            break;
        case 3:
            SP = v;
            break;
        default:
            cerr << "r16: bad index " << i << "\n";
            abort();
    }
}

uint16_t CPU::r16stk(int i) {
    switch (i) {
        case 0: return BC();
        case 1: return DE();
        case 2: return HL();
        case 3: return AF();
        default:
            cerr << "r16stk: bad index " << i << "\n";
            abort();
    }
}

void CPU::set_r16stk(int i, uint16_t v) {
    switch (i) {
        case 0:
            setBC(v);
            break;
        case 1:
            setDE(v);
            break;
        case 2:
            setHL(v);
            break;
        case 3:
            setAF(v);
            break;
        default:
            cerr << "r16stk: bad index " << i << "\n";
            abort();
    }
}

int CPU::step() {
    uint8_t op = fetch8();
    int cycles = execute(op);
    cycles_total += cycles;
    return cycles;
}

int CPU::execute(uint8_t op) {
    uint8_t x = op >> 6;
    uint8_t y = (op >> 3) & 7;
    uint8_t z = op & 7;

    switch (op) {
        case 0x00: return 4;
        case 0xF3:
            IME = false;
            ei_pending = false;
            return 4;
        case 0x76:
            halted = true;
            return 4;
        case 0xC3:
            PC = fetch16();
            return 16;
        case 0xFB:
            ei_pending = true;
            return 4;
    }

    cerr << "Unimplemented opcode 0x" << hex << int(op) << " at 0x" << (PC - 1) << "\n";
    abort();
}


