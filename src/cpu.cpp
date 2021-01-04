#include "../include/common.hpp"

Cpu::Cpu(Ram ram, uint8_t *prog_rom){
    this->prog_rom = prog_rom;
    this->ram = ram;
    this->reg = DEFAULT_REG;
    dprint("CPU construct");
}

uint8_t Cpu::fetch(){
    return this->read(this->reg.PC++);
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

uint8_t Cpu::read(uint16_t addr){
    if (addr < 0x0800) {
        // Ram
        return this->ram.read(addr);
    } else if (addr < 0x2000) {
        // mirror
        return this->ram.read(addr - 0x0800);
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
        this->ram.write(addr, data);
    } else if (addr < 0x2000) {
        // mirror
        this->ram.write(addr - 0x0800, data);
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