#include <fstream>
#include <iostream>
using namespace std;

const int NES_HEADER_SIZE = 0x0010;
const int PROGRAM_ROM_UNIT_SIZE = 0x4000;
const int CHARACTER_ROM_UNIT_SIZE = 0x2000;

class cassette {
private:
    uint8_t special[4];
    uint8_t *content;
    uint8_t *prg_rom;
    uint8_t *chr_rom;
    uint32_t prg_units;
    uint32_t chr_units;
public:
    nes_data(char* path){
        ifstream fin(path, ios::in|ios::binary);
        if (!fin){
            cout << path << " open failed!";
            // exit(1);
        }
        // get size
        fin.seekg(std::ios::end);
        size_t size = fin.tellg();
        fin.seekg(0);
        // get rom size
        fin.seekg(4);
        fin.read((char*)&prg_units, sizeof(uint32_t));
        fin.seekg(5);
        fin.read((char*)&chr_units, sizeof(uint32_t));
        cout << "prog rom size:" << prg_units << endl;
        cout << "chara rom size:" << chr_units << endl;
        // read prg rom
        uint32_t prg_size = (PROGRAM_ROM_UNIT_SIZE) * prg_units;
        fin.seekg(NES_HEADER_SIZE);
        prg_rom = new uint8_t[prg_size];
        fin.read((char*)&prg_rom, prg_size);
        // read chr rom
        uint32_t chr_size = (CHARACTER_ROM_UNIT_SIZE) * chr_units;
        fin.seekg(NES_HEADER_SIZE + prg_size);
        chr_rom = new uint8_t[chr_size];
        fin.read((char*)&chr_rom, chr_size);
    }
};

int main (int argc, char* argv[]) {
    cassette data = cassette(argv[1]);
    return 0;
}