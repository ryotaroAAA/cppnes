#include "../include/common.hpp"


cpu::cpu(){
    cout << "cpu!!!" << endl;
}

void cpu::reset(){
    // dprint("#");
    this->reg = DEFAULT_REG;
    this->reg.PC = 0xFFFC;
}