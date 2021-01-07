#include "../include/common.hpp"
#include "../include/cpu.hpp"
#include "../include/ram.hpp"

const int NES_HEADER_SIZE = 0x0010;
const int PROGRAM_ROM_UNIT_SIZE = 0x4000;
const int CHARACTER_ROM_UNIT_SIZE = 0x2000;

Cassette::Cassette(char* path){
    ifstream fin(path, ios::in|ios::binary);
    if (!fin){
        cout << path << " open failed!";
        exit(1);
    }    
    uint8_t prog_units;
    uint8_t char_units;
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
    this->prog_size = (PROGRAM_ROM_UNIT_SIZE) * prog_units;
    dprint("prog_size : %p", this->prog_size);
    fin.seekg(NES_HEADER_SIZE);
    this->prog_rom = new uint8_t[this->prog_size];
    fin.read((char*)this->prog_rom, this->prog_size);
    // read chr rom
    this->char_size = (CHARACTER_ROM_UNIT_SIZE) * char_units;
    dprint("char_size : %p", this->char_size);
    fin.seekg(NES_HEADER_SIZE + this->prog_size);
    this->char_rom = new uint8_t[this->char_size];
    fin.read((char*)this->char_rom, this->char_size);
}

uint8_t *Cassette::get_prog_rom(){
    return this->prog_rom;
}

uint8_t *Cassette::get_char_rom(){
    return this->char_rom;
}

uint16_t Cassette::get_prog_size(){
    return this->prog_size;
}

uint16_t Cassette::get_char_size(){
    return this->char_size;
}

int main (int argc, char* argv[]) {
    Cassette cas = Cassette(argv[1]);
    Ram ram = Ram();
    Cpu cpu = Cpu(&ram, &cas);
    cpu.reset();
    cpu.reg_dump();
    for(int i=0; i<1000; i++){
        cpu.run();
    }
    dprint("###");
    return 0;
}