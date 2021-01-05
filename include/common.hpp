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

// register
const uint8_t CARRY = 1<<0;
const uint8_t ZERO = 1<<1;
const uint8_t RESERVED = 1<<2;
const uint8_t BREAK = 1<<3;
const uint8_t DECIMAL = 1<<4;
const uint8_t INTERRUPT = 1<<5;
const uint8_t OVER_FLOW = 1<<6;
const uint8_t NEGATIVE = 1<<7;

typedef struct {
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint8_t P;
    uint16_t SP;
    uint16_t PC;
} reg_t;

const reg_t DEFAULT_REG = {
    0x00,
    0x00,
    0x00,
    RESERVED|BREAK|INTERRUPT,
    0x01FD,
    0x0000
};

// opecode
enum OPECODE{
    // op
    ADC, SBC,
    // logical op
    AND, ORA, EOR,
    // shift, rot
    ASL, LSR, ROL, ROR,
    // cond branch
    BCC, BCS, BEQ, BNE, BVC, BVS, BPL, BMI,
    // bit inspection
    BIT,
    // jump
    JMP, JSR, RTS,
    // interrupt
    BRK, RTI,
    // compare
    CMP, CPX, CPY,
    // inc, dec
    INC, DEC, INX, DEX, INY, DEY,
    // change flag
    CLC, SEC, CLI, SEI, CLD, SED, CLV,
    // load
    LDA, LDX, LDY,
    // store
    STA, STX, STY,
    // transfer between registers
    TAX, TXA, TAY, TYA, TSX, TXS,
    // stack
    PHA, PLA, PHP, PLP,
    // No operation
    NOP,
};

// addressing mode
enum ADDR_MODE {
    impl,
    acm,
    imd,
    zpg,
    zpg_x,
    abs,
    abs_x,
    rel,
    ind_x,
    x_ind,
    abs_ind,
};

enum OPERAND_SIZE {
    byte, word,
};

typedef struct {
    enum OPERAND_SIZE size;
    union OPERAND {
        uint8_t byte;
        uint16_t word;
    } operand;
    uint8_t add_cycle;
} operand_t;

typedef struct {
    enum OPECODE opecode;
    enum ADDR_MODE addr_mode;
    uint8_t cycle;
} opeset_t;

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
    reg_t reg;
    Ram *ram;
    uint8_t *prog_rom;
public:
    Cpu(Ram *ram, uint8_t *prog_rom);
    uint8_t run();
    void exec(opeset_t opeset);
    void reset();
    uint8_t fetch();
    void reg_dump();
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);
};

#endif //COMMON