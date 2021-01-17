#ifndef CASSETTE_H
#define CASSETTE_H

#include "../include/common.hpp"

class Cassette {
private:
    uint8_t *prog_rom;
    uint8_t *char_rom;
    uint16_t prog_size;
    uint16_t char_size;
public:
    Cassette(char* path);
    uint8_t *get_prog_rom();
    uint8_t *get_char_rom();
    uint16_t get_prog_size();
    uint16_t get_char_size();
};

#endif //CASSETTE_H