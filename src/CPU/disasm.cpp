#include "disasm.h"
#include <cstdio>

using namespace std;

static const char* R[]   = {"B", "C", "D", "E", "H", "L", "(HL)", "A"};
static const char* RP[]  = {"BC", "DE", "HL", "SP"};
static const char* RP2[] = {"BC", "DE", "HL", "AF"};
static const char* CC[]  = {"NZ", "Z", "NC", "C"};
static const char* ALU[] = {"ADD A,", "ADC A,", "SUB ", "SBC A,", "AND ", "XOR ", "OR ", "CP "};
static const char* ROT[] = {"RLC", "RRC", "RL", "RR", "SLA", "SRA", "SWAP", "SRL"};

string disasm(const Bus& bus, uint16_t pc, int& length) {
    uint8_t op = bus.read(pc);
    uint8_t n = bus.read(pc + 1);
    uint16_t nn = n | (bus.read(pc + 2) << 8);
    int8_t e = static_cast<int8_t>(n);
    // Relative jump target is measured from the end of the 2-byte instruction
    uint16_t target = pc + 2 + e;

    uint8_t x = op >> 6;
    uint8_t y = (op >> 3) & 7;
    uint8_t z = op & 7;
    uint8_t p = y >> 1;
    uint8_t q = y & 1;

    char buf[32];
    length = 1;

    if (op == 0xCB) {
        length = 2;
        uint8_t cx = n >> 6, cy = (n >> 3) & 7, cz = n & 7;
        static const char* OPS[] = {nullptr, "BIT", "RES", "SET"};
        if (cx == 0) snprintf(buf, sizeof buf, "%s %s", ROT[cy], R[cz]);
        else         snprintf(buf, sizeof buf, "%s %d,%s", OPS[cx], cy, R[cz]);
        return buf;
    }

    if (x == 0) {
        switch (z) {
            case 0:
                if (y == 0) return "NOP";
                if (y == 1) { length = 3; snprintf(buf, sizeof buf, "LD ($%04X),SP", nn); return buf; }
                if (y == 2) { length = 2; return "STOP"; }
                length = 2;
                if (y == 3) snprintf(buf, sizeof buf, "JR $%04X", target);
                else        snprintf(buf, sizeof buf, "JR %s,$%04X", CC[y - 4], target);
                return buf;
            case 1:
                if (q == 0) { length = 3; snprintf(buf, sizeof buf, "LD %s,$%04X", RP[p], nn); }
                else        snprintf(buf, sizeof buf, "ADD HL,%s", RP[p]);
                return buf;
            case 2: {
                static const char* IND[] = {"(BC)", "(DE)", "(HL+)", "(HL-)"};
                snprintf(buf, sizeof buf, q == 0 ? "LD %s,A" : "LD A,%s", IND[p]);
                return buf;
            }
            case 3:
                snprintf(buf, sizeof buf, "%s %s", q == 0 ? "INC" : "DEC", RP[p]);
                return buf;
            case 4: snprintf(buf, sizeof buf, "INC %s", R[y]); return buf;
            case 5: snprintf(buf, sizeof buf, "DEC %s", R[y]); return buf;
            case 6:
                length = 2;
                snprintf(buf, sizeof buf, "LD %s,$%02X", R[y], n);
                return buf;
            case 7: {
                static const char* MISC[] = {"RLCA", "RRCA", "RLA", "RRA", "DAA", "CPL", "SCF", "CCF"};
                return MISC[y];
            }
        }
    }

    if (x == 1) {
        if (op == 0x76) return "HALT";
        snprintf(buf, sizeof buf, "LD %s,%s", R[y], R[z]);
        return buf;
    }

    if (x == 2) {
        snprintf(buf, sizeof buf, "%s%s", ALU[y], R[z]);
        return buf;
    }

    // x == 3
    switch (z) {
        case 0:
            if (y < 4) { snprintf(buf, sizeof buf, "RET %s", CC[y]); return buf; }
            length = 2;
            if (y == 4) snprintf(buf, sizeof buf, "LDH ($FF%02X),A", n);
            if (y == 5) snprintf(buf, sizeof buf, "ADD SP,%d", e);
            if (y == 6) snprintf(buf, sizeof buf, "LDH A,($FF%02X)", n);
            if (y == 7) snprintf(buf, sizeof buf, "LD HL,SP%+d", e);
            return buf;
        case 1:
            if (q == 0) { snprintf(buf, sizeof buf, "POP %s", RP2[p]); return buf; }
            {
                static const char* MISC[] = {"RET", "RETI", "JP HL", "LD SP,HL"};
                return MISC[p];
            }
        case 2:
            if (y < 4) { length = 3; snprintf(buf, sizeof buf, "JP %s,$%04X", CC[y], nn); return buf; }
            if (y == 4) return "LD ($FF00+C),A";
            if (y == 6) return "LD A,($FF00+C)";
            length = 3;
            snprintf(buf, sizeof buf, y == 5 ? "LD ($%04X),A" : "LD A,($%04X)", nn);
            return buf;
        case 3:
            if (y == 0) { length = 3; snprintf(buf, sizeof buf, "JP $%04X", nn); return buf; }
            if (y == 6) return "DI";
            if (y == 7) return "EI";
            break;
        case 4:
            if (y < 4) { length = 3; snprintf(buf, sizeof buf, "CALL %s,$%04X", CC[y], nn); return buf; }
            break;
        case 5:
            if (q == 0) { snprintf(buf, sizeof buf, "PUSH %s", RP2[p]); return buf; }
            if (p == 0) { length = 3; snprintf(buf, sizeof buf, "CALL $%04X", nn); return buf; }
            break;
        case 6:
            length = 2;
            snprintf(buf, sizeof buf, "%s$%02X", ALU[y], n);
            return buf;
        case 7:
            snprintf(buf, sizeof buf, "RST $%02X", y * 8);
            return buf;
    }

    snprintf(buf, sizeof buf, "ILLEGAL $%02X", op);
    return buf;
}
