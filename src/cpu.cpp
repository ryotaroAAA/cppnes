#include "../include/cpu.hpp"

const int cycles[] = {
  /*0x00*/ 7, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6,
  /*0x10*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 6, 7,
  /*0x20*/ 6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 4, 4, 6, 6,
  /*0x30*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 6, 7,
  /*0x40*/ 6, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 3, 4, 6, 6,
  /*0x50*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 6, 7,
  /*0x60*/ 6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 5, 4, 6, 6,
  /*0x70*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 6, 7,
  /*0x80*/ 2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
  /*0x90*/ 2, 6, 2, 6, 4, 4, 4, 4, 2, 4, 2, 5, 5, 4, 5, 5,
  /*0xA0*/ 2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
  /*0xB0*/ 2, 5, 2, 5, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4,
  /*0xC0*/ 2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
  /*0xD0*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
  /*0xE0*/ 2, 6, 3, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
  /*0xF0*/ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
};

// opecode => opeset
map<uint8_t, opeset_t> opeset_dic{
    // load, LDA
    {0xA9, {LDA, IMD, 1}},
    {0xA5, {LDA, ZPG, 1}},
    {0xAD, {LDA, ABS, 1}},
    {0xB5, {LDA, ZPG_X, 1}},
    {0xBD, {LDA, ABS_X, 1}},
    {0xB9, {LDA, ABS_Y, 1}},
    {0xA1, {LDA, IND_X, 1}},
    {0xB1, {LDA, IND_Y, 1}},
    // load, LDX
    {0xA2, {LDX, IMD, 1}},
    {0xA6, {LDX, ZPG, 1}},
    {0xAE, {LDX, ABS, 1}},
    {0xB6, {LDX, ZPG_Y, 1}},
    {0xBE, {LDX, ABS_Y, 1}},
    // load, LDY
    {0xA0, {LDY, IMD, 1}},
    {0xA4, {LDY, ZPG, 1}},
    {0xAC, {LDY, ABS, 1}},
    {0xB4, {LDY, ZPG_X, 1}},
    {0xBC, {LDY, ABS_X, 1}},
    // store, STA
    {0x85, {STA, ZPG, 1}},
    {0x8D, {STA, ABS, 1}},
    {0x95, {STA, ZPG_X, 1}},
    {0x9D, {STA, ABS_X, 1}},
    {0x99, {STA, ABS_Y, 1}},
    {0x81, {STA, IND_X, 1}},
    {0x91, {STA, IND_Y, 1}},
    // store, STX
    {0x86, {STX, ZPG, 1}},
    {0x8E, {STX, ABS, 1}},
    {0x96, {STX, ZPG_Y, 1}},
    // store, STY
    {0x84, {STY, ZPG, 1}},
    {0x8C, {STY, ABS, 1}},
    {0x94, {STY, ZPG_X, 1}},
    // transfer between registers
    {0x84, {TXA, IMPL, 1}},
    {0x84, {TYA, IMPL, 1}},
    {0x84, {TXS, IMPL, 1}},
    {0x84, {TAY, IMPL, 1}},
    {0x84, {TAX, IMPL, 1}},
    {0x84, {TSX, IMPL, 1}},
    // stack
    {0x08, {PHP, IMPL, 1}},
    {0x28, {PLP, IMPL, 1}},
    {0x48, {PHA, IMPL, 1}},
    {0x68, {PLA, IMPL, 1}},
    // op, ADC
    {0x69, {ADC, IMD, 1}},
    {0x65, {ADC, ZPG, 1}},
    {0x6D, {ADC, ABS, 1}},
    {0x75, {ADC, ZPG_X, 1}},
    {0x7D, {ADC, ABS_X, 1}},
    {0x79, {ADC, ABS_Y, 1}},
    {0x61, {ADC, IND_X, 1}},
    {0x71, {ADC, IND_Y, 1}},
    // op, SBC
    {0xE9, {SBC, IMD, 1}},
    {0xE5, {SBC, ZPG, 1}},
    {0xED, {SBC, ABS, 1}},
    {0xF5, {SBC, ZPG_X, 1}},
    {0xFD, {SBC, ABS_X, 1}},
    {0xF9, {SBC, ABS_Y, 1}},
    {0xE1, {SBC, IND_X, 1}},
    {0xF1, {SBC, IND_Y, 1}},
    // compare, CPX
    {0xE0, {CPX, IMD, 1}},
    {0xE4, {CPX, ZPG, 1}},
    {0xEC, {CPX, ABS, 1}},
    // compare, CPY
    {0xC0, {CPY, IMD, 1}},
    {0xC4, {CPY, ZPG, 1}},
    {0xCC, {CPY, ABS, 1}},
    // compare, CMP
    {0xC9, {CMP, IMD, 1}},
    {0xC5, {CMP, ZPG, 1}},
    {0xCD, {CMP, ABS, 1}},
    {0xD5, {CMP, ZPG_X, 1}},
    {0xDD, {CMP, ABS_X, 1}},
    {0xD9, {CMP, ABS_Y, 1}},
    {0xC1, {CMP, IND_X, 1}},
    {0xD1, {CMP, IND_Y, 1}},
    // logical op, AND
    {0x29, {AND, IMD, 1}},
    {0x25, {AND, ZPG, 1}},
    {0x2D, {AND, ABS, 1}},
    {0x35, {AND, ZPG_X, 1}},
    {0x3D, {AND, ABS_X, 1}},
    {0x39, {AND, ABS_Y, 1}},
    {0x21, {AND, IND_X, 1}},
    {0x31, {AND, IND_Y, 1}},
    // logical op, EOR
    {0x49, {EOR, IMD, 1}},
    {0x45, {EOR, ZPG, 1}},
    {0x4D, {EOR, ABS, 1}},
    {0x55, {EOR, ZPG_X, 1}},
    {0x5D, {EOR, ABS_X, 1}},
    {0x59, {EOR, ABS_Y, 1}},
    {0x41, {EOR, IND_X, 1}},
    {0x51, {EOR, IND_Y, 1}},
    // logical op, ORA
    {0x09, {ORA, IMD, 1}},
    {0x05, {ORA, ZPG, 1}},
    {0x0D, {ORA, ABS, 1}},
    {0x15, {ORA, ZPG_X, 1}},
    {0x1D, {ORA, ABS_X, 1}},
    {0x19, {ORA, ABS_Y, 1}},
    {0x01, {ORA, IND_X, 1}},
    {0x11, {ORA, IND_Y, 1}},
    // bit inspection
    {0x24, {BIT, ZPG, 1}},
    {0x2C, {BIT, ABS, 1}},
    // shift rot, ASL
    {0x0A, {ASL, ACM, 1}},
    {0x06, {ASL, ZPG, 1}},
    {0x0E, {ASL, ABS, 1}},
    {0x16, {ASL, ZPG_X, 1}},
    {0x1E, {ASL, ABS_X, 1}},
    // shift rot, LSR
    {0x4A, {LSR, ACM, 1}},
    {0x46, {LSR, ZPG, 1}},
    {0x4E, {LSR, ABS, 1}},
    {0x56, {LSR, ZPG_X, 1}},
    {0x5E, {LSR, ABS_X, 1}},
    // shift rot, ROL
    {0x2A, {ROL, ACM, 1}},
    {0x26, {ROL, ZPG, 1}},
    {0x2E, {ROL, ABS, 1}},
    {0x36, {ROL, ZPG_X, 1}},
    {0x3E, {ROL, ABS_X, 1}},
    // shift rot, ROR
    {0x6A, {ROR, ACM, 1}},
    {0x66, {ROR, ZPG, 1}},
    {0x6E, {ROR, ABS, 1}},
    {0x76, {ROR, ZPG_X, 1}},
    {0x7E, {ROR, ABS_X, 1}},
    // inc
    {0xE8, {INX, IMPL, 1}},
    {0xC8, {INY, IMPL, 1}},
    {0xE6, {INC, ZPG, 1}},
    {0xEE, {INC, ABS, 1}},
    {0xF6, {INC, ZPG_X, 1}},
    {0xFE, {INC, ZPG_Y, 1}},
    // dec
    {0xCA, {DEX, IMPL, 1}},
    {0x88, {DEY, IMPL, 1}},
    {0xC6, {DEC, ZPG, 1}},
    {0xCE, {DEC, ABS, 1}},
    {0xD6, {DEC, ZPG_X, 1}},
    {0xDE, {DEC, ZPG_Y, 1}},
    // change flag
    {0x18, {CLC, IMPL, 1}},
    {0x58, {CLI, IMPL, 1}},
    {0xB8, {CLV, IMPL, 1}},
    {0x38, {SEC, IMPL, 1}},
    {0x78, {SEI, IMPL, 1}},
    // interrupt
    {0x00, {BRK, IMPL, 1}},
    {0x40, {RTI, IMPL, 1}},
    // jump
    {0x20, {JSR, ABS, 1}},
    {0x4C, {JMP, ABS, 1}},
    {0x6C, {JMP, ABS_IND, 1}},
    {0x6C, {RTS, IMPL, 1}},
    // cond branch
    {0x10, {BPL, REL, 1}},
    {0x30, {BMI, REL, 1}},
    {0x50, {BVC, REL, 1}},
    {0x70, {BVS, REL, 1}},
    {0x90, {BCC, REL, 1}},
    {0xB0, {BCS, REL, 1}},
    {0xD0, {BNE, REL, 1}},
    {0xF0, {BEQ, REL, 1}},
    // change flg
    {0xF8, {SED, IMPL, 1}},
    {0xD8, {CLD, IMPL, 1}},
    // nop
    {0xEA, {NOP, IMPL, 1}}

    // TODO:unofficials...
};

Cpu::Cpu(Ram *ram, uint8_t *prog_rom){
    this->prog_rom = prog_rom;
    this->ram = ram;
    this->reg = DEFAULT_REG;
    dprint("CPU construct");
}

DATA Cpu::fetch(enum READ_SIZE size){
    DATA data;
    if (size == BYTE) {
        data.b_data = this->read(this->reg.PC);
    } else if (size == WORD) {
        data.w_data = this->read(this->reg.PC++);
    } else if (size == NONE) {
        // dummy
    } else {
        dprint("invalid!");
        exit(1);
    }
    return data;
}

void Cpu::reset(){
    dprint("reset!!");
    this->reg = DEFAULT_REG;
    this->reg.PC = 0xFFFC;
}

void Cpu::reg_dump(){
    dump("##############");
    dump("A : %d", this->reg.A);
    dump("X : %d", this->reg.X);
    dump("Y : %d", this->reg.Y);
    dump("P : %d", this->reg.P);
    if (this->reg.P) printf("[DUMP][REG_STATUS] ");
    if (this->reg.P & CARRY) printf("CARRY ");
    if (this->reg.P & ZERO) printf("ZERO ");
    if (this->reg.P & RESERVED) printf("RESERVED ");
    if (this->reg.P & BREAK) printf("BREAK ");
    if (this->reg.P & DECIMAL) printf("DECIMAL ");
    if (this->reg.P & INTERRUPT) printf("INTERRUPT ");
    if (this->reg.P & OVER_FLOW) printf("OVER_FLOW ");
    if (this->reg.P & NEGATIVE) printf("NEGATIVE ");
    if (this->reg.P) printf("\n");
    dump("SP : %d", this->reg.SP);
    dump("PC : %d", this->reg.PC);
    dump("##############");
}

uint8_t Cpu::pop(){
    this->reg.SP++;
    return this->read(0x100 | (this->reg.SP & 0xFF));
}

void Cpu::push(uint8_t data){
    this->write(0x100 | (this->reg.SP & 0xFF), data);
    this->reg.SP--;
}

uint8_t Cpu::read(uint16_t addr){
    if (addr < 0x0800) {
        // Ram
        return this->ram->read(addr);
    } else if (addr < 0x2000) {
        // mirror
        return this->ram->read(addr - 0x0800);
    } else if (addr < 0x2008) {
        // PPU
        // TODO
    } else if (addr >= 0x4000 && addr < 0x4020) {
        if (addr == 0x4014) {
            // DMA
        } else if (addr == 0x4016) {
            // keypad
        } else {
            // APU I/O
            // this.apu.write(addr - 0x4000, data)
        }
    } else {
        // invalid
        dprint("invalid path!");
        exit(1);
    }
}

void Cpu::write(uint16_t addr, uint8_t data){
    if (addr < 0x0800) {
        // Ram
        this->ram->write(addr, data);
    } else if (addr < 0x2000) {
        // mirror
        this->ram->write(addr - 0x0800, data);
    } else if (addr < 0x2008) {
        // PPU
    } else if (addr >= 0x4000 && addr < 0x4020) {
        if (addr == 0x4014) {
            // DMA
        } else if (addr == 0x4016) {
            // keypad
        } else {
            // APU I/O
            // this.apu.write(addr - 0x4000, data)
        }
    } else {
        // invalid
        dprint("invalid path!");
        exit(1);
    }
}

opeset_t Cpu::get_opeset(uint8_t opecode){
    return opeset_dic[opecode];
}

operand_t Cpu::get_operand(opeset_t opeset){
    operand_t operand = {};
    // opesetからswitchしてoperand作る
    // runtimeのregみてcycle変えるので
    // typedef struct {
    //     enum OPERAND_SIZE size;
    //     union OPERAND {
    //         uint8_t byte;
    //         uint16_t word;
    //     } operand;
    //     uint8_t add_cycle;
    // } operand_t;
    switch(opeset.addr_mode){
        case ACM:
        case IMPL:
            // dummy
            return operand;
        case IMD:
        case ZPG: {
            operand.data = this->fetch(BYTE); 
            operand.size = BYTE;
            return operand;
        }
        case REL:
        
        case ZPG_X:
        case ZPG_Y:
        case ABS:
        case ABS_X:
        case ABS_Y:
        case IND_X:
        case IND_Y:
        case ABS_IND:
    }
    return operand;
}

void Cpu::exec(uint8_t opecode, operand_t operand){

}

uint8_t Cpu::run(){
    uint8_t opecode = this->fetch();
    opeset_t opeset = get_opeset(opecode);
    operand_t operand = get_operand(opeset);
    this->exec(opecode, operand);
    return opeset.cycle + operand.add_cycle;
}