#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: GBLatorCPP <rom file>" << endl;
        return 1;
    }

    ifstream file(argv[1], ios::binary | ios::ate);

    if (!file.is_open()) {
        cerr << "Can't open ROM file: " << argv[1] << endl;
        return 1;
    }

    streamsize size = file.tellg();

    file.seekg(0, ios::beg);

    std::vector<uint8_t> rom(size);
    file.read(reinterpret_cast<char *>(rom.data()), size);

    string title;

    for (int addr = 0x0134; addr < 0x0143; addr++) {
        uint8_t byte = rom[addr];
        if (byte == 0) {
            break;
        }

        title.push_back(byte);
    }

    cout << "Read " << rom.size() << " bytes" << endl;
    cout << title << endl;
    return 0;
}