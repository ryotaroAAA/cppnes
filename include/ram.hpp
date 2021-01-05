#ifndef RAM_H
#define RAM_H

#include "common.hpp"

class Ram {
private:
    uint8_t ram[0xFFFF];
public:
    Ram();
    void reset();
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);
};

#endif //RAM_H