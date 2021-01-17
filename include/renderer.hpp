#ifndef RENDERER_H
#define RENDERER_H

#include "common.hpp"
#include "ppu.hpp"
#include "ram.hpp"

class Renderer {
    private:
        uint16_t cycle;
        uint32_t data[256][240];
        vector<TILE> *background;
    public:
        Renderer();
        void render(IMAGE *image);
        uint32_t (*get_result())[240];
        bool should_pixel_hide(uint8_t x, uint8_t y);
        void render_background(vector<TILE> *background, vector<uint8_t> palette);
        void render_sprites(vector<SPRITE> *sprites, vector<uint8_t> palette);
        void render_sprite(SPRITE sprites, vector<uint8_t> palette);
        void render_tile(TILE tile, uint8_t tile_x, uint8_t tile_y, vector<uint8_t> palette);
};

#endif //RENDERER_H