#include "../include/ppu.hpp"

Ppu::Ppu(){

}

void Ppu::run(uint16_t cycle) {
    this->cycle = cycle;
    if (this->cycle > CYCLE_PER_LINE) {
        this->cycle -= CYCLE_PER_LINE;
        this->line++;

        if (this->line <= H_SIZE && !(this->line % TILE_SIZE)) {
            this->build_background();
        }

        if (this->line == H_SIZE_WITH_VBLANK) {
            this->line = 0;
            return;
        }
    }
}

