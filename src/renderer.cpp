#include "../include/renderer.hpp"

const vector<uint32_t> colors = {
    0x80808000, 0x003DA600, 0x0012B000, 0x44009600,
    0xA1005E00, 0xC7002800, 0xBA060000, 0x8C170000,
    0x5C2F0000, 0x10450000, 0x054A0000, 0x00472E00,
    0x00416600, 0x00000000, 0x05050500, 0x05050500,
    0xC7C7C700, 0x0077FF00, 0x2155FF00, 0x8237FA00,
    0xEB2FB500, 0xFF295000, 0xFF220000, 0xD6320000,
    0xC4620000, 0x35800000, 0x058F0000, 0x008A5500,
    0x0099CC00, 0x21212100, 0x09090900, 0x09090900,
    0xFFFFFF00, 0x0FD7FF00, 0x69A2FF00, 0xD480FF00,
    0xFF45F300, 0xFF618B00, 0xFF883300, 0xFF9C1200,
    0xFABC2000, 0x9FE30E00, 0x2BF03500, 0x0CF0A400,
    0x05FBFF00, 0x5E5E5E00, 0x0D0D0D00, 0x0D0D0D00,
    0xFFFFFF00, 0xA6FCFF00, 0xB3ECFF00, 0xDAABEB00,
    0xFFA8F900, 0xFFABB300, 0xFFD2B000, 0xFFEFA600,
    0xFFF79C00, 0xD7E89500, 0xA6EDAF00, 0xA2F2DA00,
    0x99FFFC00, 0xDDDDDD00, 0x11111100, 0x11111100
};

// const vector<uint32_t> colors = {
//     0x787878, 0x2000B0, 0x2800B8, 0x6010A0, 0x982078, 0xB01030, 0xA03000, 0x784000,
//     0x485800, 0x386800, 0x386C00, 0x306040, 0x305080, 0x000000, 0x000000, 0x000000,
//     0xB0B0B0, 0x4060F8, 0x4040FF, 0x9040F0, 0xD840C0, 0xD84060, 0xE05000, 0xC07000,
//     0x888800, 0x50A000, 0x48A810, 0x48A068, 0x4090C0, 0x000000, 0x000000, 0x000000,
//     0xFFFFFF, 0x60A0FF, 0x5080FF, 0xA070FF, 0xF060FF, 0xFF60B0, 0xFF7830, 0xFFA000,
//     0xE8D020, 0x98E800, 0x70F040, 0x70E090, 0x60D0E0, 0x787878, 0x000000, 0x000000,
//     0xFFFFFF, 0x90D0FF, 0xA0B8FF, 0xC0B0FF, 0xE0B0FF, 0xFFB8E8, 0xFFC8B8, 0xFFD8A0,
//     0xFFF090, 0xC8F080, 0xA0F0A0, 0xA0FFC8, 0xA0FFF0, 0xA0A0A0, 0x000000, 0x000000
// };

Renderer::Renderer(){
    dprint("Render construct");
    this->background = background;
}

uint32_t (*Renderer::get_result())[240] {
    return this->data;
}

void Renderer::render(IMAGE *image){
    if (image->background != nullptr) {
        this->background = image->background;
        this->render_background(image->background, image->palette);
    }
    if (image->sprites != nullptr) {
        this->render_sprites(image->sprites, image->palette);
    }
}

bool Renderer::should_pixel_hide(uint8_t x, uint8_t y){
    uint8_t tile_x = x/8;
    uint8_t tile_y = y/8;
    uint8_t background_index = tile_y *32 + tile_x;
    vector<TILE> bg = *(this->background);
    vector<vector<uint8_t>> data = bg[background_index].sprite.data;
    return !((data[y % 8][x % 8] % 4) == 0);
}

void Renderer::render_background(vector<TILE> *background, vector<uint8_t> palette){
    uint16_t count = 0;
    // background.size() = 32*30
    // dprint("background size : %d", (*background).size());
    for (TILE tile: *background) {
        uint16_t x = (count % 32) * 8;
        uint16_t y = (count / 32) * 8;
        this->render_tile(tile, x, y, palette);
        count++;
    }
}

void Renderer::render_sprites(vector<SPRITE> *sprites, vector<uint8_t> palette){
    for (SPRITE sprite: *sprites) {
        // TODO : check?
        this->render_sprite(sprite, palette);
    }
}

// write on background(tile), 8*8
void Renderer::render_sprite(SPRITE sprite, vector<uint8_t> palette){
    bool is_vertical_reverse = !!(sprite.attr & 0x80);
    bool is_horizontal_reverse =  !!(sprite.attr & 0x40);
    bool is_low_priority =  !!(sprite.attr & 0x20);
    uint8_t palette_id = sprite.attr & 0x03;
    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 0; j < 8; j++) {
            uint8_t x = sprite.x + is_horizontal_reverse ? 7 - j :j;
            uint8_t y = sprite.y + is_vertical_reverse ? 7 - i :i;
            if (is_low_priority && this->should_pixel_hide(x, y)) {
                continue;
            }
            if (sprite.data[i][j]) {
                uint8_t color_id =
                    palette[palette_id * 4 + sprite.data[i][j] + 0x10];
                // vector<uint8_t> color = colors[color_id];
                // uint16_t index = (x + y * 0x0100) * 4;
                this->data[y][x] = colors[color_id];
                // this->data[index] = color[0];
                // this->data[index + 1] = color[1];
                // this->data[index + 2] = color[2];
                // this->data[index + 3] = 0xFF;
                // dprint("(x, y) = (%d, %d)", x, y);
            }
        }
    }
}

void Renderer::render_tile(TILE tile, uint8_t tile_x, uint8_t tile_y, vector<uint8_t> palette){
    uint8_t offset_x = tile.scroll_x % 8;
    uint8_t offset_y = tile.scroll_y % 8;
    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 0; j < 8; j++) {
            uint8_t palette_index = tile.palette_id * 4 + tile.sprite.data[i][j];
            uint8_t color_id = palette[palette_index];
            // vector<uint8_t> color = colors[color_id];
            uint16_t x = tile_x + j - offset_x;
            uint16_t y = tile_y + i - offset_y;
                // dprint("(x, y) = (%d, %d) : %p", x, y, colors[color_id]);
            if (x >= 0 && x <= 255 && y >= 0 && y <= 240) {
                // uint16_t index = (x + y * 0x0100) * 4;
                this->data[y][x] =  colors[color_id];
                // this->data[index] = color[0];
                // this->data[index + 1] = color[1];
                // this->data[index + 2] = color[2];
                // this->data[index + 3] = 0xFF;
                // if (colors[color_id] != 0x05050500)
                //     dprint("(x, y) = (%d, %d), color_id:%p, color:%p, palette_index:%p"
                //         , x, y, color_id, colors[color_id], palette_index);
            }
        }
    }
}
