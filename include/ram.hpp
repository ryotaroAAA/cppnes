#ifndef RAM_H
#define RAM_H

#include "common.hpp"

class Ram {
protected:
    uint8_t *ram;
    uint16_t size;
public:
    Ram(uint16_t size);
    void reset();
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);
};

#endif //RAM_H