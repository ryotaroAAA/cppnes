#include "../include/common.hpp"

const int NES_HEADER_SIZE = 0x0010;
const int PROGRAM_ROM_UNIT_SIZE = 0x4000;
const int CHARACTER_ROM_UNIT_SIZE = 0x2000;

class cassette {
private:
    uint8_t *prg_rom;
    uint8_t *chr_rom;
    uint32_t prg_units;
    uint32_t chr_units;
public:
    cassette(char* path){
        ifstream fin(path, ios::in|ios::binary);
        if (!fin){
            cout << path << " open failed!";
            exit(1);
        }
        // get size
        fin.seekg(std::ios::end);
        size_t size = fin.tellg();
        fin.seekg(0);
        // get rom size
        fin.seekg(4);
        fin.read((char*)&prg_units, sizeof(uint8_t));
        fin.seekg(5);
        fin.read((char*)&chr_units, sizeof(uint8_t));
        dprint("prog_units : %d", prg_units);
        // dprint("chr_units : %d", chr_units);
        // read prg rom
        uint32_t prg_size = (PROGRAM_ROM_UNIT_SIZE) * prg_units;
        fin.seekg(NES_HEADER_SIZE);
        prg_rom = new uint8_t[prg_size];
        fin.read((char*)prg_rom, PROGRAM_ROM_UNIT_SIZE);
        // read chr rom
        uint32_t chr_size = (CHARACTER_ROM_UNIT_SIZE) * chr_units;
        fin.seekg(NES_HEADER_SIZE + prg_size);
        chr_rom = new uint8_t[chr_size];
        fin.read((char*)chr_rom, chr_size);
        // dprint("prog_size : %d", prg_size);
        // dprint("chr_size : %d", chr_size);
    }
};

int main (int argc, char* argv[]) {
    cassette data = cassette(argv[1]);
    cpu cp = cpu();
    cp.reset();
    cout << "success" << endl;
    return 0;
}


// #include <cstdio>

// #define DEBUG_LOG(msg, ...) std::printf("[debug] " msg "\n" __VA_OPT__(,) __VA_ARGS__)

// int main()
// {
//   DEBUG_LOG("#");       // printf("[debug] hello\n"); に展開される
//   DEBUG_LOG("value:%d", 3); // printf("[debug] value:%d\n", 3); に展開される
// }