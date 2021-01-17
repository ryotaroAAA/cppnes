#ifndef PPU_H
#define PPU_H

#include "common.hpp"
#include "ram.hpp"
#include "cassette.hpp"

  /*
    [Control Register1 0x2000]
    | bit  | description                                 |
    +------+---------------------------------------------+
    |  7   | Assert NMI when VBlank 0: disable, 1:enable |
    |  6   | PPU master/slave, always 1                  |
    |  5   | Sprite size 0: 8x8, 1: 8x16                 |
    |  4   | Bg pattern table 0:0x0000, 1:0x1000         |
    |  3   | sprite pattern table 0:0x0000, 1:0x1000     |
    |  2   | PPU memory increment 0: +=1, 1:+=32         |
    |  1-0 | Name table 0x00: 0x2000                     |
    |      |            0x01: 0x2400                     |
    |      |            0x02: 0x2800                     |
    |      |            0x03: 0x2C00                     |

    [Control Register2 0x2001]
    | bit  | description                                 |
    +------+---------------------------------------------+
    |  7-5 | Background color  0x00: Black               |
    |      |                   0x01: Green               |
    |      |                   0x02: Blue                |
    |      |                   0x04: Red                 |
    |  4   | Enable sprite                               |
    |  3   | Enable background                           |
    |  2   | Sprite mask       render left end           |
    |  1   | Background mask   render left end           |
    |  0   | Display type      0: color, 1: mono         |

    [PPU MEMORY MAP]
    | addr           |  description               |
    +----------------+----------------------------+
    | 0x0000-0x0FFF  |  Pattern table#0           |
    | 0x1000-0x1FFF  |  Pattern table#1           |
    | 0x2000-0x23BF  |  Name table                |
    | 0x23C0-0x23FF  |  Attribute table           |
    | 0x2400-0x27BF  |  Name table                |
    | 0x27C0-0x27FF  |  Attribute table           |
    | 0x2800-0x2BBF  |  Name table                |
    | 0x2BC0-0x2BFF  |  Attribute table           |
    | 0x2C00-0x2FBF  |  Name Table                |
    | 0x2FC0-0x2FFF  |  Attribute Table           |
    | 0x3000-0x3EFF  |  mirror of 0x2000-0x2EFF   |
    | 0x3F00-0x3F0F  |  background Palette        |
    | 0x3F10-0x3F1F  |  sprite Palette            |
    | 0x3F20-0x3FFF  |  mirror of 0x3F00-0x3F1F   |
*/

const uint16_t SPRITE_RAM_SIZE = 0x0100;
const uint16_t PALETTE_SIZE = 0x20;
const uint16_t VRAM_SIZE = 0x0800;
const uint8_t TILE_SIZE = 8;
const uint8_t V_SPRITE_NUM = 30;
const uint8_t H_SPRITE_NUM = 32;
const uint16_t H_SIZE = 240;
const uint16_t V_SIZE = 256;
const uint16_t H_SIZE_WITH_VBLANK = 262;
const uint16_t CYCLE_PER_LINE = 341;

typedef struct {
    uint8_t PPUCTRL;
    uint8_t PPUMASK;
    uint8_t PPUSTATUS;
    uint8_t OAMADDR;
    uint8_t OAMDATA;
    uint8_t PPUSCROLL;
    uint8_t PPUADDR;
    uint8_t PPUDATA;
} PPUREG;

typedef struct {
    vector<vector<uint8_t>> data;
    uint16_t x;
    uint16_t y;
    uint16_t attr;
} SPRITE;

typedef struct {
    SPRITE sprite;
    uint8_t palette_id;
    uint8_t scroll_x;
    uint8_t scroll_y;
    uint8_t sprite_id;
} TILE;

typedef struct {
    vector<SPRITE> *sprites;
    vector<TILE> *background;
    vector<uint8_t> palette;
} IMAGE;

class Palette : public Ram {
    private:
    public:
        Palette();        
        Palette(uint16_t size);        
        bool is_sprite_mirror(uint8_t addr);
        bool is_background_mirror(uint8_t addr);
        uint8_t get_palette_addr(uint16_t addr);
        vector<uint8_t> read();
        void write(uint16_t addr, uint8_t data);
};

class Ppu {
    private:
        // variables
        Cassette *cas;
        Palette palette;
        PPUREG reg;
        Ram *vram;
        Ram charactor_ram;
        uint8_t vram_buf;
        uint16_t vram_addr;
        uint16_t vram_offset;
        Ram sprite_ram;
        uint16_t sprite_ram_addr;
        uint16_t cycle;
        uint16_t line;
        uint8_t tile_x;
        uint8_t tile_y;
        uint8_t scroll_x;
        uint8_t scroll_y;
        vector<SPRITE> sprites;
        vector<TILE> background;
        IMAGE image;
        bool is_horizontal_scroll;
        bool is_lower_vram_addr;
        // methods
        uint8_t get_scroll_tile_x();
        uint8_t get_scroll_tile_y();
        uint8_t get_tile_y();
        uint8_t get_block_id(uint8_t x, uint8_t y);
        uint8_t get_sprite_id(uint8_t x, uint8_t y, uint16_t offset);
        uint8_t get_attribute(uint8_t x, uint8_t y, uint16_t offset);
        vector<uint8_t> get_palette();
        uint8_t get_vram_offset();
        uint8_t get_name_table_id();
        bool has_vblank_irq_enabled();
        uint8_t get_background_table_offset();
        bool get_is_background_enable();
        bool get_is_sprite_enable();
        bool has_sprite_hit();
        void set_sprite_hit();
        void clear_sprite_hit();
        void set_vblank();
        bool get_is_vblank();
        void clear_vblank();
        uint16_t calc_vram_addr();
        uint8_t vram_read();
        void write_sprite_ram_addr(uint8_t data);
        void write_sprite_ram_data(uint8_t data);
        void write_scroll_data(uint8_t data);
        void write_vram_addr(uint8_t data);
        void write_vram_data(uint8_t data);
        uint16_t get_block_id();
        uint16_t get_sprite_id();
        uint16_t get_parette();
        TILE build_tile(uint8_t x, uint8_t y, uint16_t offset);
        void build_sprites();
        vector<vector<uint8_t>> build_sprite_data(uint8_t sprite_id, uint16_t offset);
        void build_background();
    public:
        Ppu(Ram *vram, Cassette *cas);
        IMAGE *run(uint16_t cycle);
        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t data);
};

#endif //PPU_H