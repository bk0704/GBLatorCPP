
#include "cpu.h"

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