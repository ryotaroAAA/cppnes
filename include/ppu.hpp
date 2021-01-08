#ifndef PPU_H
#define PPU_H

#include "common.hpp"
#include "ram.hpp"

const uint8_t TILE_SIZE = 8;
const uint8_t V_SPRITE_NUM = 32;
const uint8_t H_SPRITE_NUM = 30;
const uint8_t H_SIZE = 240;
const uint16_t H_SIZE_WITH_VBLANK = 262;
const uint16_t CYCLE_PER_LINE = 341;

class Ppu {
    private:
        Casette *cas;
        uint16_t cycle;
        uint16_t line;
        uint16_t tile_x;
        uint16_t tile_y;
        
    public:
        Ppu();
        void run(uint16_t cycle);
        uint16_t get_block_id();
        uint16_t get_sprite_id();
        uint16_t get_parette();
        void build_tile();
        void build_sprite();
        void build_background();
};

#endif //PPU_H