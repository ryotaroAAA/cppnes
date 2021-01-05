#ifndef CPU_H
#define CPU_H

#include "common.hpp"
#include "ram.hpp"

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
} REG;

const REG DEFAULT_REG = {
    0x00,
    0x00,
    0x00,
    RESERVED|BREAK|INTERRUPT,
    0x01FD,
    0x0000
};

// opecode
typedef enum {
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
} OPECODE;

// addressing mode
typedef enum {
    IMPL,
    ACM,
    IMD,
    ZPG,
    ZPG_X,
    ZPG_Y,
    ABS,
    ABS_X,
    ABS_Y,
    REL,
    IND_X,
    IND_Y,
    ABS_IND,
} ADDR_MODE;

typedef struct {
    SIZE size;
    DATA data;
    uint8_t add_cycle;
} OPERAND;

typedef struct {
    OPECODE opecode;
    ADDR_MODE addr_mode;
    uint8_t cycle;
} OPESET;

class Cpu {
private:
    REG reg;
    Ram *ram;
    uint8_t *prog_rom;
public:
    Cpu(Ram *ram, uint8_t *prog_rom);
    uint8_t run();
    void exec(uint8_t opecode, OPERAND operand);
    DATA fetch(SIZE size);
    uint8_t pop();
    void push(uint8_t data);
    DATA read(uint16_t addr, SIZE size);
    void write(uint16_t addr, DATA data);
    OPESET get_opeset(uint8_t opecode);
    OPERAND get_operand(OPESET opeset);
    void reset();
    void reg_dump();
};

#endif //CPU_H