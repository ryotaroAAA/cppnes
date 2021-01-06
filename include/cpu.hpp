#ifndef CPU_H
#define CPU_H

#include "common.hpp"
#include "ram.hpp"

// status register
typedef struct {
    uint8_t carry;
    uint8_t zero;
    uint8_t reserved;
    uint8_t brk;
    uint8_t decimal;
    uint8_t interrupt;
    uint8_t overflow;
    uint8_t negative;
} STATUS_REG;

typedef struct {
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    STATUS_REG P;
    uint16_t SP;
    uint16_t PC;
} REG;

const STATUS_REG DEFAULT_STATUS_REG = {
    0, 0, 1, 1, 0, 1, 0, 0
};

const REG DEFAULT_REG = {
    0x00,
    0x00,
    0x00,
    DEFAULT_STATUS_REG,
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
    bool has_branched;
public:
    Cpu(Ram *ram, uint8_t *prog_rom);
    uint8_t run();
    void exec(OPESET opeset, OPERAND operand);
    DATA fetch(SIZE size);
    void branch(uint16_t addr);
    uint8_t pop();
    void push(uint8_t data);
    void pop_PC();
    void push_reg_status();
    void pop_reg_status();
    DATA read(uint16_t addr, SIZE size);
    uint8_t read_(uint16_t addr);
    void write(uint16_t addr, DATA data);
    OPESET get_opeset(uint8_t opecode);
    OPERAND get_operand(OPESET opeset);
    void set_reg_zero_neg(uint8_t result);
    void reset();
    void reg_dump();
    void op_dump();
};

#endif //CPU_H