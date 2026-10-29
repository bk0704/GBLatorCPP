#include "bus.h"
#include "cartridge.h"

using namespace std;
Bus::Bus(const Cartridge &cart) {
    size_t count = min(cart.size(), size_t{0x8000});

    for (size_t i = 0; i < count; i++) {
        mem[i] = cart.read(static_cast<uint16_t>(i));
    }
}
