#include "cpu.h"
#include <iostream>

using namespace std;
int CPU::execute(uint8_t op) {
    uint8_t x = op >> 6;
    uint8_t y = (op >> 3) & 7;
    uint8_t z = op & 7;



    cerr << "Unimplemented opcode 0x" << hex << int(op) << " at 0x" << (PC - 1) << "\n";
    abort();
}