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

void Ppu:build_tile(uint16_t x, uint16_t y){
    uint16_t block_id = this->get_block_id(x, y);
    uint16_t sprite_id = this->get_sprite_id(x, y);
    uint16_t palette = this->get_palette(x, y);
    uint16_t palette_id = (palette >> (block_id * 2)) & 0x03;
    this->build_sprite(sprite_id);
}

void Ppu:build_sprite(uint8_t sprite_id){
    for (uint8_t i = 0; x < 16; i++) {
        for (uint8_t j = 0; x < 8; j++) {
            uint8_t addr = sprite_id * 16 + i;
            uint8_t *ram = this->cas->get_char_rom();
            if (ram & (0x80 >> j)) {
                sprite[i % 8][j] +=
                    0x01 << i/8
            }
        }
    }
}

void Ppu:build_background(){
    uint8_t y = this->tile_y % H_SPRITE_NUM;
    for (uint8_t x = 0; x < V_SPRITE_NUM; x++) {
        uint16_t name_table_id = (x/32);
    }
}