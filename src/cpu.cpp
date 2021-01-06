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
map<uint8_t, OPESET> opeset_dic{
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
    this->reg.PC = 0xFFFC;
}

void Cpu::reg_dump(){
    dump("##############");
    dump("A : %d", this->reg.A);
    dump("X : %d", this->reg.X);
    dump("Y : %d", this->reg.Y);
    dump("P : %d", this->reg.P);
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
    dump("SP : %d", this->reg.SP);
    dump("PC : %d", this->reg.PC);
    dump("##############");
}

void Cpu::branch(uint16_t addr){
    this->reg.PC = addr;
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

void Cpu::write(uint16_t addr, DATA data){
    if (addr < 0x0800) {
        // Ram
        this->ram->write(addr, data.b_data);
    } else if (addr < 0x2000) {
        // mirror
        this->ram->write(addr - 0x0800, data.b_data);
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

OPESET Cpu::get_opeset(uint8_t opecode){
    return opeset_dic[opecode];
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
                (operand.data.b_data & 0xFF00 != this->reg.PC & 0xFF00) ? 1 : 0;
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
                (operand.data.w_data != this->reg.X & 0xFF00) ? 1 : 0;
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
            this->write(data.b_data, _data);
            break;
        }
        case STX : {
            DATA _data;
            _data.b_data = this->reg.X;
            this->write(data.b_data, _data);
            break;
        }
        case STY : {
            DATA _data;
            _data.b_data = this->reg.Y;
            this->write(data.b_data, _data);
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
            uint16_t temp = this->reg.A + _data + this->reg.P.carry;
            this->reg.P.carry = temp > 0xFF;
            // TODO
            this->reg.P.overflow = (!(((this->reg.A ^ _data) & 0x80) != 0) &&
                (((this->reg.A ^ temp) & 0x80)) != 0);
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
        case LSR : {}
        case ORA : {}
        case ROL : {}
        case ROR : {}
        case SBC : {}
        case PHA : {
            this->push(this->reg.A);
        }
        case PHP : {}
        case PLA : {}
        case PLP : {}
        case JMP : {
            this->reg.PC = data.w_data;
            break;
        }
        case JSR : {}
        case RTS : {}
        case RTI : {}
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
            // TODO
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
    }
}

uint8_t Cpu::run(){
    uint8_t opecode = this->fetch(BYTE).b_data;
    OPESET opeset = get_opeset(opecode);
    OPERAND operand = get_operand(opeset);
    this->exec(opeset, operand);
    return opeset.cycle + operand.add_cycle;
}