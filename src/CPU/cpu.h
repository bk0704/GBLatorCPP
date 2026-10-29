#pragma once
#ifndef GBLATORCPP_CPU_H
#define GBLATORCPP_CPU_H
#include <cstdint>
#include <cstdio>
#include "../bus.h"


class CPU {
public:
    explicit CPU(Bus& bus) : bus(bus) {}
    void reset();
    int step();

    uint8_t A{}, F{}, B{}, C{}, D{}, E{}, H{}, L{};
    uint16_t SP{}, PC{};
    bool IME{}, halted{}, stopped{}, ei_pending{};
    uint64_t cycles_total{};

    static constexpr uint8_t FLAG_Z = 0x80, FLAG_N = 0x40, FLAG_H = 0x20, FLAG_C = 0x10;
    void log_state(FILE* out);

private:
    Bus& bus;

    uint8_t fetch8() { return bus.read(PC++); }
    uint16_t fetch16() {
        uint8_t low = fetch8();
        uint16_t high = fetch8();
        return (high << 8) | low;
    }
    int8_t fetch_e8() { return static_cast<int8_t>(fetch8()); }

    uint16_t AF() const { return (A << 8) | F;}
    uint16_t BC() const { return (B << 8) | C;}
    uint16_t DE() const { return (D << 8) | E;}
    uint16_t HL() const { return (H << 8) | L;}
    void setAF(uint16_t v) { A = v >> 8; F = v & 0xF0; }
    void setBC(uint16_t v) { B = v >> 8; C = v & 0xFF; }
    void setDE(uint16_t v) { D = v >> 8; E = v & 0xFF; }
    void setHL(uint16_t v) { H = v >> 8; L = v & 0xFF; }

    bool flag(uint8_t m) const {return F & m; }
    void setFlag(uint8_t m, bool on) { F = on ? (F | m) : (F & ~m); F &= 0xF0; }

    uint8_t  r8(int i);
    void set_r8(int i, uint8_t v);
    uint16_t r16(int i);
    void set_r16(int i, uint16_t v);
    uint16_t r16stk(int i);
    void set_r16stk(int i, uint16_t v);

    int execute(uint8_t op);
    int execute_cb(uint8_t op);
};


#endif //GBLATORCPP_CPU_H