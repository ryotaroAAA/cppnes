#ifndef COMMON
#define COMMON

#include <fstream>
#include <iostream>
#include <cstddef>
#include <cstdlib> 
#include <cstdio>
#include <string.h>
using namespace std;

#define dprint(msg, ...) printf("[DEBUG] " msg " %s(%d) @ %s\n", ##__VA_ARGS__  ,__func__, __LINE__, __FILE__)
#define dump(msg, ...) printf("[DUMP] " msg "\n", ##__VA_ARGS__)

const uint8_t CARRY = 1<<0;
const uint8_t ZERO = 1<<1;
const uint8_t RESERVED = 1<<2;
const uint8_t BREAK = 1<<3;
const uint8_t DECIMAL = 1<<4;
const uint8_t INTERRUPT = 1<<5;
const uint8_t OVER_FLOW = 1<<6;
const uint8_t NEGATIVE = 1<<7;

struct reg {
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint8_t P;
    uint16_t SP;
    uint16_t PC;
};

const reg DEFAULT_REG = {
    0x00,
    0x00,
    0x00,
    RESERVED|BREAK|INTERRUPT,
    0x01FD,
    0x0000
};

class Ram {
private:
    uint8_t ram[0xFFFF];
public:
    Ram();
    void reset();
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);
};

class Cpu {
private:
    struct reg reg;
    Ram ram;
    uint8_t *prog_rom;
public:
    Cpu(Ram ram, uint8_t *prog_rom);
    void run();
    void exec();
    void reset();
    uint8_t fetch();
    void reg_dump();
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);
};

#endif //COMMON