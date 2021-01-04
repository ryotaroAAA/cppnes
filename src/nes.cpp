#include "../include/common.hpp"

const int NES_HEADER_SIZE = 0x0010;
const int PROGRAM_ROM_UNIT_SIZE = 0x4000;
const int CHARACTER_ROM_UNIT_SIZE = 0x2000;

class Cassette {
private:
    uint8_t *prog_rom;
    uint8_t *char_rom;
    uint8_t prog_units;
    uint8_t char_units;
public:
    Cassette(char* path){
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
        fin.read((char*)&prog_units, sizeof(uint8_t));
        fin.seekg(5);
        fin.read((char*)&char_units, sizeof(uint8_t));
        dprint("prog_units : %d", prog_units);
        dprint("char_units : %d", char_units);
        // read prg rom
        uint32_t prog_size = (PROGRAM_ROM_UNIT_SIZE) * prog_units;
        dprint("prog_size : %d", prog_size);
        fin.seekg(NES_HEADER_SIZE);
        prog_rom = new uint8_t[prog_size];
        fin.read((char*)prog_rom, PROGRAM_ROM_UNIT_SIZE);
        // read chr rom
        uint32_t char_size = (CHARACTER_ROM_UNIT_SIZE) * char_units;
        dprint("char_size : %d", char_size);
        fin.seekg(NES_HEADER_SIZE + prog_size);
        char_rom = new uint8_t[char_size];
        fin.read((char*)char_rom, char_size);
    }
    uint8_t *get_prog_rom(){
        return this->prog_rom;
    }
    uint8_t *get_char_rom(){
        return this->char_rom;
    }
};

int main (int argc, char* argv[]) {
    Cassette cassette = Cassette(argv[1]);
    Ram ram = Ram();
    Cpu cpu = Cpu(ram, cassette.get_prog_rom());
    cpu.reg_dump();
    cpu.reset();
    dprint("###");
    return 0;
}