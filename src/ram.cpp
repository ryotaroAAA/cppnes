#include "../include/ram.hpp"

Ram::Ram(){
    dprint("Ram construct");
}

Ram::Ram(uint16_t size){
    dprint("Ram construct size:%p, %u", size, size);
    this->size = size;
    this->ram = new uint8_t[this->size];
    for(int i = 0; i < this->size; i++){
        this->ram[i] = 0;
    }
}

void Ram::dump_ram(){
    for (int i = 0; i < this->size; i++) {
        dprint("ram_addr:%p, data:%p %d", i, this->ram[i], this->ram[i]);
    }
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