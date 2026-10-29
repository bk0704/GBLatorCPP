#pragma once
#ifndef GBLATORCPP_DISASM_H
#define GBLATORCPP_DISASM_H
#include <cstdint>
#include <string>
#include "../bus.h"

// Decodes the instruction at pc into text like "LD HL,$C000".
// length is set to the instruction's size in bytes (1-3).
std::string disasm(const Bus& bus, uint16_t pc, int& length);

#endif //GBLATORCPP_DISASM_H
