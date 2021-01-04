#ifndef COMMON
#define COMMON

#include <fstream>
#include <iostream>
#include <cstddef>
#include <cstdint>
using namespace std;

#define dprint(...) printf("[DEBUG] %s(%d) @ %s\n", __func__, __LINE__, __FILE__)
// #define dprint(msg, ...) printf("ggg" msg "ggg\n" __VA_OPT__(,) __VA_ARGS__)
// #define dprint(...) cout << "[debug] " << __VA_ARGS__ << "hoge" << endl
const uint8_t CARRY = 0x00;
const uint8_t ZERO = 0x01;
const uint8_t RESERVED = 0x02;
const uint8_t BREAK = 0x03;
const uint8_t DECIMAL = 0x04;
const uint8_t INTERRUPT = 0x05;
const uint8_t OVERFLOW = 0x06;
const uint8_t NEGATIVE = 0x07;

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

class cpu {
private:
    reg reg;
public:
    cpu();
    void reset();
};

#endif //COMMON