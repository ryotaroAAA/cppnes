#include "../include/common.hpp"

Ram::Ram(){
    dprint("Ram construct");
    memset(this->ram, 0, sizeof(this->ram));
}

void Ram::reset(){
    dprint("reset!!");
    memset(this->ram, 0, sizeof(this->ram));
}

uint8_t Ram::read(uint16_t addr){
    return this->ram[addr];
}

void Ram::write(uint16_t addr, uint8_t value){
    this->ram[addr] = value;
}