#include "../include/cpu.hpp"

const uint8_t cycles[] = {
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

map<ADDR_MODE, const char*> addrmode_dic{
    {IMPL, "IMPL"}, 
    {ACM, "ACM"}, 
    {IMD, "IMD"}, 
    {ZPG, "ZPG"}, 
    {ZPG_X, "ZPG_X"}, 
    {ZPG_Y, "ZPG_Y"}, 
    {ABS, "ABS"}, 
    {ABS_X, "ABS_X"}, 
    {ABS_Y, "ABS_Y"}, 
    {REL, "REL"}, 
    {IND_X, "IND_X"}, 
    {IND_Y, "IND_Y"}, 
    {ABS_IND, "ABS_IND"}
};

// opecode => opeset
map<uint8_t, OPESET> opeset_dic {
    // load, LDA
    {0xA9, {LDA, "LDA", IMD, cycles[0xA9]}},
    {0xA5, {LDA, "LDA", ZPG, cycles[0xA5]}},
    {0xAD, {LDA, "LDA", ABS, cycles[0xAD]}},
    {0xB5, {LDA, "LDA", ZPG_X, cycles[0xB5]}},
    {0xBD, {LDA, "LDA", ABS_X, cycles[0xBD]}},
    {0xB9, {LDA, "LDA", ABS_Y, cycles[0xB9]}},
    {0xA1, {LDA, "LDA", IND_X, cycles[0xA1]}},
    {0xB1, {LDA, "LDA", IND_Y, cycles[0xB1]}},
    // load, LDX
    {0xA2, {LDX, "LDX", IMD, cycles[0xA2]}},
    {0xA6, {LDX, "LDX", ZPG, cycles[0xA6]}},
    {0xAE, {LDX, "LDX", ABS, cycles[0xAE]}},
    {0xB6, {LDX, "LDX", ZPG_Y, cycles[0xB6]}},
    {0xBE, {LDX, "LDX", ABS_Y, cycles[0xBE]}},
    // load, LDY
    {0xA0, {LDY, "LDY", IMD, cycles[0xA0]}},
    {0xA4, {LDY, "LDY", ZPG, cycles[0xA4]}},
    {0xAC, {LDY, "LDY", ABS, cycles[0xAC]}},
    {0xB4, {LDY, "LDY", ZPG_X, cycles[0xB4]}},
    {0xBC, {LDY, "LDY", ABS_X, cycles[0xBC]}},
    // store, STA
    {0x85, {STA, "STA", ZPG, cycles[0x85]}},
    {0x8D, {STA, "STA", ABS, cycles[0x8D]}},
    {0x95, {STA, "STA", ZPG_X, cycles[0x95]}},
    {0x9D, {STA, "STA", ABS_X, cycles[0x9D]}},
    {0x99, {STA, "STA", ABS_Y, cycles[0x99]}},
    {0x81, {STA, "STA", IND_X, cycles[0x81]}},
    {0x91, {STA, "STA", IND_Y, cycles[0x91]}},
    // store, STX
    {0x86, {STX, "STX", ZPG, cycles[0x86]}},
    {0x8E, {STX, "STX", ABS, cycles[0x8E]}},
    {0x96, {STX, "STX", ZPG_Y, cycles[0x96]}},
    // store, STY
    {0x84, {STY, "STY", ZPG, cycles[0x84]}},
    {0x8C, {STY, "STY", ABS, cycles[0x8C]}},
    {0x94, {STY, "STY", ZPG_X, cycles[0x94]}},
    // transfer between registers
    {0x8A, {TXA, "TXA", IMPL, cycles[0x8A]}},
    {0x98, {TYA, "TYA", IMPL, cycles[0x98]}},
    {0x9A, {TXS, "TXS", IMPL, cycles[0x9A]}},
    {0xA8, {TAY, "TAY", IMPL, cycles[0xA8]}},
    {0xAA, {TAX, "TAX", IMPL, cycles[0xAA]}},
    {0xB8, {TSX, "TSX", IMPL, cycles[0xB8]}},
    // stack
    {0x08, {PHP, "PHP", IMPL, cycles[0x08]}},
    {0x28, {PLP, "PLP", IMPL, cycles[0x28]}},
    {0x48, {PHA, "PHA", IMPL, cycles[0x48]}},
    {0x68, {PLA, "PLA", IMPL, cycles[0x68]}},
    // op, ADC
    {0x69, {ADC, "ADC", IMD, cycles[0x69]}},
    {0x65, {ADC, "ADC", ZPG, cycles[0x65]}},
    {0x6D, {ADC, "ADC", ABS, cycles[0x6D]}},
    {0x75, {ADC, "ADC", ZPG_X, cycles[0x75]}},
    {0x7D, {ADC, "ADC", ABS_X, cycles[0x7D]}},
    {0x79, {ADC, "ADC", ABS_Y, cycles[0x79]}},
    {0x61, {ADC, "ADC", IND_X, cycles[0x61]}},
    {0x71, {ADC, "ADC", IND_Y, cycles[0x71]}},
    // op, SBC
    {0xE9, {SBC, "SBC", IMD, cycles[0xE9]}},
    {0xE5, {SBC, "SBC", ZPG, cycles[0xE5]}},
    {0xED, {SBC, "SBC", ABS, cycles[0xED]}},
    {0xF5, {SBC, "SBC", ZPG_X, cycles[0xF5]}},
    {0xFD, {SBC, "SBC", ABS_X, cycles[0xFD]}},
    {0xF9, {SBC, "SBC", ABS_Y, cycles[0xF9]}},
    {0xE1, {SBC, "SBC", IND_X, cycles[0xE1]}},
    {0xF1, {SBC, "SBC", IND_Y, cycles[0xF1]}},
    // compare, CPX
    {0xE0, {CPX, "CPX", IMD, cycles[0xE0]}},
    {0xE4, {CPX, "CPX", ZPG, cycles[0xE4]}},
    {0xEC, {CPX, "CPX", ABS, cycles[0xEC]}},
    // compare, CPY
    {0xC0, {CPY, "CPY", IMD, cycles[0xC0]}},
    {0xC4, {CPY, "CPY", ZPG, cycles[0xC4]}},
    {0xCC, {CPY, "CPY", ABS, cycles[0xCC]}},
    // compare, CMP
    {0xC9, {CMP, "CMP", IMD, cycles[0xC9]}},
    {0xC5, {CMP, "CMP", ZPG, cycles[0xC5]}},
    {0xCD, {CMP, "CMP", ABS, cycles[0xCD]}},
    {0xD5, {CMP, "CMP", ZPG_X, cycles[0xD5]}},
    {0xDD, {CMP, "CMP", ABS_X, cycles[0xDD]}},
    {0xD9, {CMP, "CMP", ABS_Y, cycles[0xD9]}},
    {0xC1, {CMP, "CMP", IND_X, cycles[0xC1]}},
    {0xD1, {CMP, "CMP", IND_Y, cycles[0xD1]}},
    // logical op, AND
    {0x29, {AND, "AND", IMD, cycles[0x29]}},
    {0x25, {AND, "AND", ZPG, cycles[0x25]}},
    {0x2D, {AND, "AND", ABS, cycles[0x2D]}},
    {0x35, {AND, "AND", ZPG_X, cycles[0x35]}},
    {0x3D, {AND, "AND", ABS_X, cycles[0x3D]}},
    {0x39, {AND, "AND", ABS_Y, cycles[0x39]}},
    {0x21, {AND, "AND", IND_X, cycles[0x21]}},
    {0x31, {AND, "AND", IND_Y, cycles[0x31]}},
    // logical op, EOR
    {0x49, {EOR, "EOR", IMD, cycles[0x49]}},
    {0x45, {EOR, "EOR", ZPG, cycles[0x45]}},
    {0x4D, {EOR, "EOR", ABS, cycles[0x4D]}},
    {0x55, {EOR, "EOR", ZPG_X, cycles[0x55]}},
    {0x5D, {EOR, "EOR", ABS_X, cycles[0x5D]}},
    {0x59, {EOR, "EOR", ABS_Y, cycles[0x59]}},
    {0x41, {EOR, "EOR", IND_X, cycles[0x41]}},
    {0x51, {EOR, "EOR", IND_Y, cycles[0x51]}},
    // logical op, ORA
    {0x09, {ORA, "ORA", IMD, cycles[0x09]}},
    {0x05, {ORA, "ORA", ZPG, cycles[0x05]}},
    {0x0D, {ORA, "ORA", ABS, cycles[0x0D]}},
    {0x15, {ORA, "ORA", ZPG_X, cycles[0x15]}},
    {0x1D, {ORA, "ORA", ABS_X, cycles[0x1D]}},
    {0x19, {ORA, "ORA", ABS_Y, cycles[0x19]}},
    {0x01, {ORA, "ORA", IND_X, cycles[0x01]}},
    {0x11, {ORA, "ORA", IND_Y, cycles[0x11]}},
    // bit inspection
    {0x24, {BIT, "BIT", ZPG, cycles[0x24]}},
    {0x2C, {BIT, "BIT", ABS, cycles[0x2C]}},
    // shift rot, ASL
    {0x0A, {ASL, "ASL", ACM, cycles[0x0A]}},
    {0x06, {ASL, "ASL", ZPG, cycles[0x06]}},
    {0x0E, {ASL, "ASL", ABS, cycles[0x0E]}},
    {0x16, {ASL, "ASL", ZPG_X, cycles[0x16]}},
    {0x1E, {ASL, "ASL", ABS_X, cycles[0x1E]}},
    // shift rot, LSR
    {0x4A, {LSR, "LSR", ACM, cycles[0x4A]}},
    {0x46, {LSR, "LSR", ZPG, cycles[0x46]}},
    {0x4E, {LSR, "LSR", ABS, cycles[0x4E]}},
    {0x56, {LSR, "LSR", ZPG_X, cycles[0x56]}},
    {0x5E, {LSR, "LSR", ABS_X, cycles[0x5E]}},
    // shift rot, ROL
    {0x2A, {ROL, "ROL", ACM, cycles[0x2A]}},
    {0x26, {ROL, "ROL", ZPG, cycles[0x26]}},
    {0x2E, {ROL, "ROL", ABS, cycles[0x2E]}},
    {0x36, {ROL, "ROL", ZPG_X, cycles[0x36]}},
    {0x3E, {ROL, "ROL", ABS_X, cycles[0x3E]}},
    // shift rot, ROR
    {0x6A, {ROR, "ROR", ACM, cycles[0x6A]}},
    {0x66, {ROR, "ROR", ZPG, cycles[0x66]}},
    {0x6E, {ROR, "ROR", ABS, cycles[0x6E]}},
    {0x76, {ROR, "ROR", ZPG_X, cycles[0x76]}},
    {0x7E, {ROR, "ROR", ABS_X, cycles[0x7E]}},
    // inc
    {0xE8, {INX, "INX", IMPL, cycles[0xE8]}},
    {0xC8, {INY, "INY", IMPL, cycles[0xC8]}},
    {0xE6, {INC, "INC", ZPG, cycles[0xE6]}},
    {0xEE, {INC, "INC", ABS, cycles[0xEE]}},
    {0xF6, {INC, "INC", ZPG_X, cycles[0xF6]}},
    {0xFE, {INC, "INC", ZPG_Y, cycles[0xFE]}},
    // dec
    {0xCA, {DEX, "DEX", IMPL, cycles[0xCA]}},
    {0x88, {DEY, "DEY", IMPL, cycles[0x88]}},
    {0xC6, {DEC, "DEC", ZPG, cycles[0xC6]}},
    {0xCE, {DEC, "DEC", ABS, cycles[0xCE]}},
    {0xD6, {DEC, "DEC", ZPG_X, cycles[0xD6]}},
    {0xDE, {DEC, "DEC", ZPG_Y, cycles[0xDE]}},
    // change flag
    {0x18, {CLC, "CLC", IMPL, cycles[0x18]}},
    {0x58, {CLI, "CLI", IMPL, cycles[0x58]}},
    {0xB8, {CLV, "CLV", IMPL, cycles[0xB8]}},
    {0x38, {SEC, "SEC", IMPL, cycles[0x38]}},
    {0x78, {SEI, "SEI", IMPL, cycles[0x78]}},
    // interrupt
    {0x00, {BRK, "BRK", IMPL, cycles[0x00]}},
    {0x40, {RTI, "RTI", IMPL, cycles[0x40]}},
    {0x60, {RTS, "RTS", IMPL, cycles[0x60]}},
    // jump
    {0x20, {JSR, "JSR", ABS, cycles[0x20]}},
    {0x4C, {JMP, "JMP", ABS, cycles[0x4C]}},
    {0x6C, {JMP, "JMP", ABS_IND, cycles[0x6C]}},
    // cond branch
    {0x10, {BPL, "BPL", REL, cycles[0x10]}},
    {0x30, {BMI, "BMI", REL, cycles[0x30]}},
    {0x50, {BVC, "BVC", REL, cycles[0x50]}},
    {0x70, {BVS, "BVS", REL, cycles[0x70]}},
    {0x90, {BCC, "BCC", REL, cycles[0x90]}},
    {0xB0, {BCS, "BCS", REL, cycles[0xB0]}},
    {0xD0, {BNE, "BNE", REL, cycles[0xD0]}},
    {0xF0, {BEQ, "BEQ", REL, cycles[0xF0]}},
    // change flg
    {0xF8, {SED, "SED", IMPL, cycles[0xF8]}},
    {0xD8, {CLD, "CLD", IMPL, cycles[0xD8]}},
    // nop
    {0xEA, {NOP, "NOP", IMPL, cycles[0xEA]}}

    // TODO:unofficials...
};

Cpu::Cpu(Ram *ram, Cassette *cas, Ppu *ppu){
    this->cas = cas;
    this->ram = ram;
    this->ppu = ppu;
    this->reg = DEFAULT_REG;
    dprint("CPU construct");
}

DATA Cpu::fetch(SIZE size){
    DATA data;
    if (size == BYTE) {
        data.b_data = this->read(this->reg.PC, BYTE).b_data;
        this->reg.PC++;
    } else if (size == WORD) {
        data.w_data = this->read(this->reg.PC, WORD).w_data;
        this->reg.PC += 2;
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
    this->reg.PC = this->read(0xFFFC, WORD).w_data;
}

void Cpu::reg_dump(){
    dump("##############");
    dump("A : 0x%p", this->reg.A);
    dump("X : 0x%p", this->reg.X);
    dump("Y : 0x%p", this->reg.Y);
    dump("P : 0x%p", this->reg.P);
    printf("[DUMP][REG_STATUS] ");
    if (this->reg.P.carry) printf("CARRY ");
    if (this->reg.P.zero) printf("ZERO ");
    if (this->reg.P.reserved) printf("RESERVED ");
    if (this->reg.P.brk) printf("BREAK ");
    if (this->reg.P.decimal) printf("DECIMAL ");
    if (this->reg.P.interrupt) printf("INTERRUPT ");
    if (this->reg.P.overflow) printf("OVER_FLOW ");
    if (this->reg.P.negative) printf("NEGATIVE ");
    printf("\n");
    dump("SP : 0x%p", this->reg.SP);
    dump("PC : 0x%p", this->reg.PC);
    dump("##############");
}

void Cpu::branch(uint16_t addr){
    this->reg.PC = addr-1;
    this->has_branched = true;
}

uint8_t Cpu::pop(){
    this->reg.SP++;
    return this->read(0x100 | (this->reg.SP & 0xFF), BYTE).b_data;
}

void Cpu::push(uint8_t data){
    DATA _data;
    _data.b_data = data;
    this->write(0x100 | (this->reg.SP & 0xFF), _data);
    this->reg.SP--;
}

void Cpu::pop_PC(){
    this->reg.PC = this->pop();
    this->reg.PC += (this->pop() << 8);
}

void Cpu::push_PC(){
    this->push((this->reg.PC >> 8) & 0xFF);
    this->push(this->reg.PC & 0xFF);
}

void Cpu::push_reg_status(){
    uint8_t status = 
        (!!this->reg.P.carry) |
        (!!this->reg.P.zero << 1) |
        (!!this->reg.P.reserved << 2) |
        (!!this->reg.P.brk << 3) |
        (!!this->reg.P.decimal << 4) |
        (!!this->reg.P.interrupt << 5) |
        (!!this->reg.P.overflow << 6) |
        (!!this->reg.P.negative << 7);
    this->push(status);
}

void Cpu::pop_reg_status(){
    uint8_t status = this->pop();
    this->reg.P.carry = !!(status & (1<<0));
    this->reg.P.zero = !!(status & (1<<1));
    this->reg.P.reserved = !!(status & (1<<2));
    this->reg.P.brk = !!(status & (1<<3));
    this->reg.P.decimal = !!(status & (1<<4));
    this->reg.P.interrupt = !!(status & (1<<5));
    this->reg.P.overflow = !!(status & (1<<6));
    this->reg.P.negative = !!(status & (1<<7));
}

DATA Cpu::read(uint16_t addr, SIZE size){
    DATA data;
    addr &= 0xFFFF;
    if (size == WORD) {
        data.w_data = this->read_(addr) | (this->read_(addr + 1) << 8);
    } else if (size == BYTE) {
        data.b_data = this->read_(addr);
    } else if (size == NONE) {
        // dummy
    } else {
        // invalid
        dprint("invalid path!");
        exit(1);
    }
    return data;
}

uint8_t Cpu::read_(uint16_t addr){
    if (addr < 0x0800) {
        // Ram
        return this->ram->read(addr);
    } else if (addr < 0x2000) {
        // mirror
        return this->ram->read(addr - 0x0800);
    } else if (addr < 0x4000) {
        // PPU
        // dprint("read:%p, %d", addr, addr);
        return this->ppu->read((addr - 0x2000) % 8);
    } else if (addr >= 0x4000 && addr < 0x4020) {
        if (addr == 0x4014) {
            // DMA
        } else if (addr == 0x4016) {
            // keypad
        } else {
            // APU I/O
            // this.apu.write(addr - 0x4000, data)
        }
    } else if (addr >= 0x4020 && addr < 0x6000) {
        // ext rom
    } else if (addr >= 0x6000 && addr < 0x8000) {
        // ext ram
    } else if (addr >= 0x8000 && addr < 0xFFFF) {
        // prog rom
        if (addr < 0xC000) {
            return this->cas->get_prog_rom()[addr - 0x8000];
        } else {
            if (this->cas->get_prog_size() < 0x4000) {
                return this->cas->get_prog_rom()[addr - 0xC000];
            } else {
                return this->cas->get_prog_rom()[addr - 0x8000];
            }
        }
    } else {
        // invalid
        dprint("invalid path!");
        exit(1);
    }
}

void Cpu::write(uint16_t addr, DATA data){
    // dprint("write : %p : %d", addr, data.w_data);
    if (addr < 0x0800) {
        // Ram
        this->ram->write(addr, data.b_data);
    } else if (addr < 0x2000) {
        // mirror
        this->ram->write(addr - 0x0800, data.b_data);
    } else if (addr < 0x2008) {
        // PPU
        this->ppu->write(addr - 0x2000, data.b_data);
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

OPERAND Cpu::get_operand(OPESET opeset){
    OPERAND operand = {};
    switch(opeset.addr_mode){
        case ACM:
        case IMPL:
            // dummy
            break;
        case IMD:
        case ZPG: {
            operand.data = this->fetch(BYTE); 
            operand.size = BYTE;
            break;
        }
        case REL: {
            uint8_t reladdr = this->fetch(BYTE).b_data; 
            operand.data.w_data = (reladdr < 0x80)?
                reladdr + this->reg.PC:
                reladdr + this->reg.PC - 0xFF; 
            operand.size = BYTE;
            operand.add_cycle = 
                ((operand.data.w_data & 0xFF00) != (this->reg.PC & 0xFF00)) ? 1 : 0;
            break;
        }
        case ZPG_X: {
            operand.data.b_data = 
                (this->reg.X + this->fetch(BYTE).b_data) & 0xFF; 
            operand.size = BYTE;
            break;
        }
        case ZPG_Y: {
            operand.data.b_data = 
                (this->reg.Y + this->fetch(BYTE).b_data) & 0xFF; 
            operand.size = BYTE;
            break;
        }
        case ABS: {
            operand.data = this->fetch(WORD); 
            operand.size = WORD;
            break;
        }
        case ABS_X: {
            operand.data.w_data = 
                (this->reg.X + this->fetch(WORD).w_data) & 0xFFFF; 
            operand.size = WORD;
            operand.add_cycle = 
                (operand.data.w_data != (this->reg.X & 0xFF00)) ? 1 : 0;
            break;
        }
        case ABS_Y: {
            operand.data.w_data = 
                (this->reg.X + this->fetch(WORD).w_data) & 0xFFFF; 
            operand.size = WORD;
            operand.add_cycle = 
                (operand.data.w_data != this->reg.Y & 0xFF00) ? 1 : 0;
            break;
        }
        case IND_X:{
            uint8_t base = (this->fetch(BYTE).b_data + this->reg.X) & 0xFF;
            operand.data.w_data = this->read(base, BYTE).b_data +
                (this->read(base + 1, BYTE).b_data << 8); 
            operand.size = WORD;
            operand.add_cycle = 
                (operand.data.w_data & 0xFF00 != base & 0xFF00) ? 1 : 0;
            break;
        }
        case IND_Y:{
            uint16_t base = this->fetch(BYTE).b_data; 
            base = this->read(base + 1, BYTE).b_data +
                (this->read(base, BYTE).b_data << 8) + this->reg.Y;
            operand.data.w_data = base + this->reg.Y;
            operand.size = WORD;
            operand.add_cycle = 
                (operand.data.w_data & 0xFF00 != base & 0xFF00) ? 1 : 0;
            break;
        }
        case ABS_IND:{
            uint16_t base = this->fetch(BYTE).b_data; 
            base = this->read(base + 1, BYTE).b_data +
                (this->read(base, BYTE).b_data << 8) + this->reg.Y;
            operand.data.w_data = base + this->reg.Y;
            operand.size = WORD;
            operand.add_cycle = 
                (operand.data.w_data & 0xFF00 != base & 0xFF00) ? 1 : 0;
            break;
        }
        default:{
            dprint("invalid!");
            exit(1);
        }
    }
    return operand;
}

void Cpu::set_reg_zero_neg(uint8_t result){
    this->reg.P.negative = !!(result & 0x80);
    this->reg.P.zero = !result;
}

void Cpu::exec(OPESET opeset, OPERAND operand){
    this->has_branched = false;
    ADDR_MODE mode = opeset.addr_mode;
    DATA data = operand.data;
    switch(opeset.opecode){
        // load
        case LDA : {
            this->reg.A = (mode == IMD)?
                data.b_data : this->read(data.w_data, BYTE).b_data;
            this->set_reg_zero_neg(this->reg.A);
            break;
        }
        case LDX : {
            this->reg.X = (mode == IMD)?
                data.b_data : this->read(data.w_data, BYTE).b_data;
            this->set_reg_zero_neg(this->reg.X);
            break;
        }
        case LDY : {
            this->reg.Y = (mode == IMD)?
                data.b_data : this->read(data.w_data, BYTE).b_data;
            this->set_reg_zero_neg(this->reg.Y);
            break;
        }
        // store
        case STA : {
            DATA _data;
            _data.b_data = this->reg.A;
            this->write(data.w_data, _data);
            break;
        }
        case STX : {
            DATA _data;
            _data.b_data = this->reg.X;
            this->write(data.w_data, _data);
            break;
        }
        case STY : {
            DATA _data;
            _data.b_data = this->reg.Y;
            this->write(data.w_data, _data);
            break;
        }
        // transfer
        case TAX : {
            this->reg.X = this->reg.A;
            this->set_reg_zero_neg(this->reg.X);
            break;
        }
        case TAY : {
            this->reg.Y = this->reg.A;
            this->set_reg_zero_neg(this->reg.Y);
            break;
        }
        case TSX : {
            this->reg.X = this->reg.SP & 0xFF;
            this->set_reg_zero_neg(this->reg.X);
            break;
        }
        case TXA : {
            this->reg.A = this->reg.X;
            this->set_reg_zero_neg(this->reg.A);
            break;
        }
        case TXS : {
            this->reg.SP = this->reg.X + 0x0100;
            break;
        }
        case TYA : {
            this->reg.A = this->reg.Y;
            this->set_reg_zero_neg(this->reg.A);
            break;
        }
        // op
        case ADC : {
            uint8_t _data = (mode == IMD)?
                data.b_data : this->read(data.w_data, BYTE).b_data;
            uint16_t temp = 
                this->reg.A + _data + this->reg.P.carry;
            this->reg.P.carry = temp > 0xFF;
            // TODO
            this->reg.P.overflow =
                ((this->reg.A ^ _data) & 0x80) &&
                !((this->reg.A ^ temp) & 0x80);
            this->set_reg_zero_neg(temp);
            this->reg.A = temp & 0xFF;
            break;
        }
        case AND : {
            uint8_t _data = (mode == IMD)?
                data.b_data : this->read(data.w_data, BYTE).b_data;
            this->reg.A &= _data;
            this->set_reg_zero_neg(this->reg.A);
            break;
        }
        case ASL : {
            if (mode == ACM) {
                uint16_t _data = this->reg.A;
                this->reg.P.carry = !!(_data & 0x80);
                this->reg.A = (_data << 1) & 0xFF;
                this->set_reg_zero_neg(this->reg.A);
            } else {
                uint16_t _data = this->read(data.w_data, BYTE).b_data;
                this->reg.P.carry = !!(_data & 0x80);
                _data = (_data << 1) & 0xFF;
                DATA temp;
                temp.w_data = _data;
                this->write(data.w_data, temp);
                this->set_reg_zero_neg(_data);
            }
            break;
        }
        case BIT : {
            uint8_t _data = this->read(data.w_data, BYTE).b_data;
            this->reg.P.overflow = !!(_data & 0x40);
            this->reg.P.negative = !!(_data & 0x80);
            this->reg.P.zero = !(this->reg.A & _data);
        }
        case CMP : {
            uint8_t _data = (mode == IMD)?
                data.b_data : this->read(data.w_data, BYTE).b_data;
            int comp = this->reg.A - _data;
            this->reg.P.carry = (comp >= 0);
            this->set_reg_zero_neg(comp);
            break;
        }
        case CPX : {
            uint8_t _data = (mode == IMD)?
                data.b_data : this->read(data.w_data, BYTE).b_data;
            int comp = this->reg.X - _data;
            this->reg.P.carry = (comp >= 0);
            this->set_reg_zero_neg(comp);
            break;
        }
        case CPY : {
            uint8_t _data = (mode == IMD)?
                data.b_data : this->read(data.w_data, BYTE).b_data;
            int comp = this->reg.Y - _data;
            this->reg.P.carry = (comp >= 0);
            this->set_reg_zero_neg(comp);
            break;
        }
        case DEC : {
            uint8_t _data = this->read(data.w_data, BYTE).b_data - 1;
            DATA temp;
            temp.w_data = _data;
            this->write(data.w_data, temp);
            this->set_reg_zero_neg(_data);
            break;
        }
        case DEX : {
            this->reg.X--;
            this->set_reg_zero_neg(this->reg.X);
            break;
        }
        case DEY : {
            this->reg.Y--;
            this->set_reg_zero_neg(this->reg.Y);
            break;
        }
        case EOR : {
            uint8_t _data = (mode == IMD)?
                data.b_data : this->read(data.w_data, BYTE).b_data;
            this->reg.A ^= _data;
            this->set_reg_zero_neg(this->reg.A);
            break;
        }
        case INC : {
            uint8_t _data = this->read(data.w_data, BYTE).b_data + 1;
            DATA temp;
            temp.w_data = _data;
            this->write(data.w_data, temp);
            this->set_reg_zero_neg(_data);
            break;
        }
        case INX : {
            this->reg.X++;
            this->set_reg_zero_neg(this->reg.X);
            break;
        }
        case INY : {
            this->reg.Y++;
            this->set_reg_zero_neg(this->reg.Y);
            break;
        }
        case LSR : {
            if (mode == ACM) {
                this->reg.P.carry = !!(this->reg.A & 0x01);
                this->reg.A = this->reg.A >> 1;
                this->reg.P.zero = !this->reg.A;
            } else {
                uint8_t _data = this->read(data.w_data, BYTE).b_data;
                this->reg.P.carry = !!(_data & 0x01);
                this->reg.P.zero = !(_data >> 1);
                DATA temp;
                temp.b_data = _data >> 1;
                this->write(data.w_data, temp);
            }
            this->reg.P.negative = false;
            break;
        }
        case ORA : {
            uint8_t _data = (mode == IMD)?
                data.b_data : this->read(data.w_data, BYTE).b_data;
            this->reg.A |= _data;
            this->set_reg_zero_neg(this->reg.A);
            break;
        }
        case ROL : {
            if (mode == ACM) {
                this->reg.A = (this->reg.A << 1) | !!(this->reg.P.carry);
                this->reg.P.carry = !!(this->reg.A & 0x80);
                this->set_reg_zero_neg(this->reg.A);
            } else {
                uint8_t _data = this->read(data.w_data, BYTE).b_data;
                uint8_t result = (_data << 1) | !!(this->reg.P.carry);
                DATA temp;
                temp.b_data = result;
                this->write(data.w_data, temp);
                this->reg.P.carry = !!(_data & 0x80);
                this->set_reg_zero_neg(result);
            }
            this->reg.P.negative = false;
            break;
        }
        case ROR : {
            if (mode == ACM) {
                this->reg.A = (this->reg.A >> 1) |
                    (this->reg.P.carry) ? 0x80 : 0x00;
                this->reg.P.carry = !!(this->reg.A & 0x01);
                this->set_reg_zero_neg(this->reg.A);
            } else {
                uint8_t _data = this->read(data.w_data, BYTE).b_data;
                uint8_t result = (_data >> 1) |
                    (this->reg.P.carry) ? 0x80 : 0x00;
                DATA temp;
                temp.b_data = result;
                this->write(data.w_data, temp);
                this->reg.P.carry = !!(_data & 0x01);
                this->set_reg_zero_neg(result);
            }
            break;
        }
        case SBC : {
            uint8_t _data = (mode == IMD)?
                data.b_data : this->read(data.w_data, BYTE).b_data;
            uint16_t temp = 
                this->reg.A - _data - !this->reg.P.carry;
            if (_data != 0xFF){
                this->reg.P.carry =
                    this->reg.A >= (_data + !this->reg.P.carry);
            } else {
                this->reg.P.carry = false;
            }
            // TODO
            this->reg.P.overflow =
                ((this->reg.A ^ _data) & 0x80) &&
                !((this->reg.A ^ temp) & 0x80);
            this->set_reg_zero_neg(temp);
            this->reg.A = temp & 0xFF;
            break;
        }
        case PHA : {
            this->push(this->reg.A);
        }
        case PHP : {
            this->reg.P.brk = true;
            this->push_reg_status();
        }
        case PLA : {
            this->reg.A = this->pop();
            this->set_reg_zero_neg(this->reg.A);
        }
        case PLP : {
            this->pop_reg_status();
            this->reg.P.reserved = true;
        }
        case JMP : {
            this->reg.PC = data.w_data;
            break;
        }
        case JSR : {
            uint16_t pc = this->reg.PC - 1;
            this->push((pc >> 8) & 0xFF);
            this->push(pc & 0xFF);
            this->reg.PC = data.w_data;
        }
        case RTS : {
            this->pop_PC();
            this->reg.PC++;
        }
        case RTI : {
            this->pop_reg_status();
            this->pop_PC();
            this->reg.P.reserved = true;
        }
        case BCC : {
            if (!this->reg.P.carry) {
                this->branch(data.w_data);
            }
            break;
        }
        case BCS : {
            if (this->reg.P.carry) {
                this->branch(data.w_data);
            }
            break;
        }
        case BEQ : {
            if (this->reg.P.zero) {
                this->branch(data.w_data);
            }
            break;
        }
        case BMI : {
            if (this->reg.P.negative) {
                this->branch(data.w_data);
            }
            break;
        }
        case BNE : {
            if (!this->reg.P.zero) {
                // dprint("%p", data.w_data);
                this->branch(data.w_data);
            }
            break;
        }
        case BPL : {
            if (!this->reg.P.negative) {
                this->branch(data.w_data);
            }
            break;
        }
        case BVS : {
            if (this->reg.P.overflow) {
                this->branch(data.w_data);
            }
            break;
        }
        case BVC : {
            if (!this->reg.P.overflow) {
                this->branch(data.w_data);
            }
            break;
        }
        case CLD : {
            this->reg.P.decimal = 0;
            break;
        }
        case CLC : {
            this->reg.P.carry = 0;
            break;
        }
        case CLI : {
            this->reg.P.interrupt = 0;
            break;
        }
        case CLV : {
            this->reg.P.overflow = 0;
            break;
        }
        case SEC : {
            this->reg.P.carry = 1;
            break;
        }
        case SEI : {
            this->reg.P.interrupt = 1;
            break;
        }
        case SED : {
            this->reg.P.decimal = 1;
            break;
        }
        case BRK : {
            this->reg.PC++;
            this->push_PC();
            this->push_reg_status();
            if (!this->reg.P.interrupt){
                this->reg.PC = this->read(0xFFFE, WORD).w_data;
            }
            this->reg.P.interrupt = true;
            this->reg.PC--;
            break;
        }
        case NOP : {
            break;
        }
        // TODO:unofficial
        // case NOPD : {}
        // case NOPI : {}
        // case LAX : {}
        // case SAX : {}
        // case DCP : {}
        // case ISB : {}
        // case SLO : {}
        // case RLA : {}
        // case SRE : {}
        // case RRA : {} 
        default : {
            dprint("invalid op!");
            exit(1);
        }
    }
}

OPESET Cpu::get_opeset(uint8_t opecode){
    // printf("%p", opecode);
    return opeset_dic[opecode];
}

void Cpu::op_dump(OPESET opeset, OPERAND operand){
    uint16_t data = (operand.size == WORD) ? 
        operand.data.w_data : operand.data.b_data;
    // printf("[OP_DUMP][PC:0x%x][%s %s 0x%x]\n", this->reg.PC, opeset.name, addrmode_dic[opeset.addr_mode], data);
}

uint8_t Cpu::run(){
    uint8_t opecode = this->fetch(BYTE).b_data;
    OPESET opeset = get_opeset(opecode);
    OPERAND operand = get_operand(opeset);
    this->op_dump(opeset, operand);
    this->exec(opeset, operand);
    return opeset.cycle + operand.add_cycle;
}
