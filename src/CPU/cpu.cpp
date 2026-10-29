
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
    bool enableAfter = ei_pending;
    ei_pending = false;

    uint8_t op = fetch8();
    int cycles = execute(op);
    cycles_total += cycles;

    if (enableAfter) IME = true;
    return cycles;
}

void CPU::log_state(FILE *out) {
    fprintf(out,
        "A:%02X F:%02X B:%02X C:%02X D:%02X E:%02X H:%02X L:%02X SP:%04X PC:%04X PCMEM:%02X,%02X,%02X,%02X\n",
        A, F, B, C, D, E, H, L, SP, PC,
        bus.read(PC), bus.read(PC + 1), bus.read(PC + 2), bus.read(PC + 3));
}



