#include "../include/ram.hpp"

Ram::Ram(uint16_t size){
    dprint("Ram construct");
    this->size = size;
    this->ram = new uint8_t[this->size];
    memset(this->ram, 0, sizeof(this->size));
}

void Ram::reset(){
    dprint("reset!!");
    memset(this->ram, 0, sizeof(this->size));
}

uint8_t Ram::read(uint16_t addr){
    return this->ram[addr];
}

void Ram::write(uint16_t addr, uint8_t value){
    this->ram[addr] = value;
}