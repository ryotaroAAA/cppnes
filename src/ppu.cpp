#include "../include/ppu.hpp"

const vector<vector<uint8_t>> default_sprite_data(8, vector<uint8_t>(8, 0));
const SPRITE default_sprite = {default_sprite_data, 0, 0, 0};
const vector<SPRITE> default_sprites(64, default_sprite);

const TILE default_tile = {default_sprite, 0, 0, 0};
const vector<TILE> default_background(16, default_tile);

Palette::Palette() : Ram(){}

Palette::Palette(uint16_t size) : Ram(size){
    dprint("palette constract!");
}

vector<uint8_t> Palette::read(){
    vector<uint8_t> data(PALETTE_SIZE, 0x00);
    for (uint8_t i = 0x00; i < data.size(); i++) {
        if (this->is_sprite_mirror(i)) {
            data[i] = this->ram[i - 0x10];
        }
        if (this->is_background_mirror(i)) {
            data[i] = this->ram[0x00];
        }
        data[i] = this->ram[i];
    }
    return data;
}

void Palette::write(uint16_t addr, uint8_t data){
    this->ram[this->get_palette_addr(addr)] = data;
}

bool Palette::is_sprite_mirror(uint8_t addr){
    return (addr == 0x10) ||
        (addr == 0x14) ||
        (addr == 0x18) ||
        (addr == 0x1c);
}

bool Palette::is_background_mirror(uint8_t addr){
    return (addr == 0x04) ||
        (addr == 0x08) ||
        (addr == 0x0c);
}

uint8_t Palette::get_palette_addr(uint16_t addr){
    uint8_t mirror_downed = addr % 0x20;
    return this->is_sprite_mirror(mirror_downed) ?
        mirror_downed - 0x10 : mirror_downed;
}

Ppu::Ppu(Ram *vram, Cassette *cas){
    dprint("PPU construct!");
    this->vram = vram;
    this->cas = cas;
    this->palette = Palette(PALETTE_SIZE);
    this->charactor_ram = Ram(cas->get_char_size());
    this->sprite_ram = Ram(SPRITE_RAM_SIZE);
    for (uint16_t i = 0x00; i < cas->get_char_size(); i++) {
        this->charactor_ram.write(i, this->cas->get_char_rom()[i]); 
    }
    this->sprites = default_sprites;
    this->reg = {};
    this->vram_buf = 0;
    this->vram_addr = 0x0000;
    this->vram_offset = 0x0000;
    this->sprite_ram_addr = 0x0000;
    this->scroll_x = 0;
    this->scroll_x = 0;
    is_horizontal_scroll = true;
    is_lower_vram_addr = false;
    vram_buf = 0;
}

// Control Register 1, PPU memory increment
uint8_t Ppu::get_vram_offset() {
    return (this->reg.PPUCTRL & 0x04) ? 32 : 1;
}

// Control Register 1, Main Screen assignment by name table
uint8_t Ppu::get_name_table_id() {
    return (this->reg.PPUCTRL & 0x03);
}

// Control Register 1, Assert NMI when VBlank
bool Ppu::has_vblank_irq_enabled() {
    return !!(this->reg.PPUCTRL & 0x80);
}

// Control Register 1, get background pattern table
uint8_t Ppu::get_background_table_offset() {
    return (this->reg.PPUCTRL & 0x10) ? 0x1000 : 0x0000;
}

// Control Register 2, Enable sprite
bool Ppu::get_is_background_enable() {
    return !!(this->reg.PPUMASK & 0x08);
}

// Control Register 2, Enable sprite
bool Ppu::get_is_sprite_enable() {
    return !!(this->reg.PPUMASK & 0x10);
}

// PPU status register
void Ppu::set_sprite_hit(){
    this->reg.PPUMASK |= 0x40;
}

// PPU status register
void Ppu::clear_sprite_hit(){
    this->reg.PPUMASK &= 0xBF;
}

// PPU status register
void Ppu::set_vblank(){
    this->reg.PPUSTATUS |= 0x80;
}

// PPU status register
bool Ppu::get_is_vblank() {
    return !!(this->reg.PPUSTATUS & 0x80);
}

// PPU status register
void Ppu::clear_vblank(){
    this->reg.PPUSTATUS &= 0x7F;
}

bool Ppu::has_sprite_hit() {
    // main screen y
    uint8_t y = this->sprite_ram.read(0x00);
    return (y == this->line) &&
        this->get_is_background_enable() &&
        this->get_is_sprite_enable();
}

uint8_t Ppu::get_scroll_tile_x(){
    return this->scroll_x + ((this->get_name_table_id() % 2) * 256) / 8;
}

uint8_t Ppu::get_scroll_tile_y(){
    return this->scroll_y + ((this->get_name_table_id() / 2) * 240) / 8;
}

uint8_t Ppu::get_tile_y(){
    return (this->line / 8) + this->get_scroll_tile_y() - 1;
}

uint8_t Ppu::get_block_id(uint8_t x, uint8_t y){
    return ((x % 4) / 2 + (y % 4) / 2) * 2;
}

// read from name_table
uint8_t Ppu::get_sprite_id(uint8_t x, uint8_t y, uint16_t offset){
    uint16_t tile_num = y * 32 + x;
    uint16_t sprite_addr = tile_num + offset;
    // uint16_t sprite_addr = tile_num + offset;
    uint8_t temp = this->vram->read(sprite_addr);
    // dprint("addr %p %d", sprite_addr, temp);
    return temp;
    // return this->vram->read(sprite_addr);
}

uint8_t Ppu::get_attribute(uint8_t x, uint8_t y, uint16_t offset){
    uint16_t addr = (x / 4) + (y / 4) * 8 + 0x23C0 + offset;
    // TODO
    // return this->vram->read(this->mirror_down_sprite_addr(addr));
    uint8_t temp = this->vram->read(addr);
    // dprint("(%d, %d) addr:%p, data:%p %d", x, y, addr, temp, temp);
    return temp;
}

vector<uint8_t> Ppu::get_palette() {
    return this->palette.read();
}

uint16_t Ppu::calc_vram_addr(){
    return (this->vram_addr >= 0x3000 && this->vram_addr < 0x3F00)?
        this->vram_addr -= 0x3000:
        this->vram_addr - 0x2000;
}

uint8_t Ppu::vram_read(){
    uint8_t buf = this->vram_buf;
    if (this->vram_addr >= 0x2000) {
        // name table, attribute table, pallette
        uint16_t addr = this->calc_vram_addr();
        this->vram_addr += this->get_vram_offset();
        // dprint("addr %p %d", addr, buf);
        if (addr >= 0x3F00) {
            // palette
            return this->vram->read(addr);
        }
    } else {
        // pattern table from charactor rom
        this->vram_buf = this->charactor_ram.read(this->vram_addr);
        this->vram_addr += this->get_vram_offset();
        // dprint("addr %p %d", this->vram_addr, this->vram_buf);
    }
    return buf;
}

uint8_t Ppu::read(uint16_t addr){
    /*
    | bit  | description                                 |
    +------+---------------------------------------------+
    | 7    | 1: VBlank clear by reading this register    |
    | 6    | 1: sprite hit                               |
    | 5    | 0: less than 8, 1: 9 or more                |
    | 4-0  | invalid                                     |                                 
    |      | bit4 VRAM write flag [0: success, 1: fail]  |
    */
    if (addr == 0x0002) {
        // PPUSTATUS
        this->is_horizontal_scroll = true;
        this->clear_vblank();
        return this->reg.PPUSTATUS;
    } else if (addr == 0x0004) {
        // OAMADDR
        // TODO?
        dprint("TODO!");
        return this->sprite_ram.read(this->sprite_ram_addr);
    } else if (addr == 0x0007) {
        // PPUDATA
        return this->vram_read();
    }
    // else {
    //     dprint("invalid path!");
    //     exit(1);
    // }
}

void Ppu::write_sprite_ram_addr(uint8_t data){
    this->sprite_ram_addr = data;
}

void Ppu::write_sprite_ram_data(uint8_t data){
    // dprint("addr:%d %p, data %d", this->sprite_ram_addr, this->sprite_ram_addr, data);    
    this->sprite_ram.write(this->sprite_ram_addr, data);
    this->sprite_ram_addr++;
}

void Ppu::write_scroll_data(uint8_t data){
    if (this->is_horizontal_scroll) {
        this->is_horizontal_scroll = false;
        this->scroll_x = data & 0xFF;
    } else {
        this->scroll_y = data & 0xFF;
        this->is_horizontal_scroll = true;
    }
}

void Ppu::write_vram_addr(uint8_t data){
    if (this->is_lower_vram_addr) {
        // dprint("low add %p", data);
        this->vram_addr += data;
        this->is_lower_vram_addr = false;
    } else {
        // dprint("high add %p", data<<8);
        this->vram_addr = data << 8;
        this->is_lower_vram_addr = true;
    }
}

void Ppu::write_vram_data(uint8_t data){
    // dprint("%p : %p %d", this->vram_addr, data, data);
    if (this->vram_addr >= 0x2000) {
        if (this->vram_addr >= 0x3F00 && this->vram_addr < 0x4000) {
            // pallette
            this->palette.write(this->vram_addr - 0x3F00, data);
        } else {
            // name table, attr table
            this->vram->write(this->calc_vram_addr(), data);
        }
    } else {
        // pattern table from charactor rom
        this->charactor_ram.write(this->vram_addr, data);
    }
    this->vram_addr += this->get_vram_offset();
}

void Ppu::write(uint16_t addr, uint8_t data){
    // dprint("%p : %p", addr, data);

    if (addr == 0x0003) {
        // set sprite ram write addr
        // dprint("addr:%d, %p", addr);
        this->write_sprite_ram_addr(data);
    } else if (addr == 0x0004) {
        // sprite ram write
        // dprint("data:%d, %p", addr);
        this->write_sprite_ram_data(data);
    } else if (addr == 0x0005) {
        // set scroll setting
        this->write_scroll_data(data);
    } else if (addr == 0x0006) {
        // set vram write addr (first: high 8bit, second: low 8bit)
        this->write_vram_addr(data);
    } else if (addr == 0x0007) {
        // sprite ram write
        this->write_vram_data(data);
    }
    // else {
    //     dprint("invalid path!");
    //     exit(1);
    // }
}

vector<vector<uint8_t>> Ppu::build_sprite_data(uint8_t sprite_id, uint16_t offset){
    /* 
        Bit Planes                  Pixel Pattern (return value)
        [lower bit]
        $0xx0=$41  01000001
        $0xx1=$C2  11000010
        $0xx2=$44  01000100
        $0xx3=$48  01001000
        $0xx4=$10  00010000
        $0xx5=$20  00100000         .1.....3
        $0xx6=$40  01000000         11....3.
        $0xx7=$80  10000000  =====  .1...3..
        [higher bit]                .1..3...
        $0xx8=$01  00000001  =====  ...3.22.
        $0xx9=$02  00000010         ..3....2
        $0xxA=$04  00000100         .3....2.
        $0xxB=$08  00001000         3....222
        $0xxC=$16  00010110
        $0xxD=$21  00100001
        $0xxE=$42  01000010
        $0xxF=$87  10000111

        see https://wiki.nesdev.com/w/index.php/PPU_pattern_tables
    */
    vector<vector<uint8_t>> sprite(8, vector<uint8_t>(8, 0));
    for (uint8_t i = 0; i < 16; i++) {
        uint16_t addr = sprite_id * 16 + i + offset;
        // dprint("[charactor_ram read] addr:%p, data:%d", addr, this->charactor_ram.read(addr));
        uint16_t ram = this->charactor_ram.read(addr);
        ram = this->cas->get_char_rom()[addr];
        for (uint8_t j = 0; j < 8; j++) {
            if (ram & (0x80 >> j)) {
                sprite[i % 8][j] += 0x01 << i/8;
            }
        }
    }

    return sprite;
}

void Ppu::build_sprites(){
    // see https://wiki.nesdev.com/w/index.php/PPU_OAM
    uint16_t offset = (this->reg.PPUCTRL & 0x08) ? 0x1000 : 0x0000;
    for (uint16_t i = 0; i < SPRITE_RAM_SIZE; i += 4) {
        this->sprites[i/4].y = this->sprite_ram.read(i) - 8;
        uint8_t sprite_id = this->sprite_ram.read(i + 1);
        // dprint("[%d] sprite_id : %d", i, sprite_id);
        this->sprites[i/4].attr = this->sprite_ram.read(i + 2);
        this->sprites[i/4].x = this->sprite_ram.read(i + 3);
        this->sprites[i/4].data = this->build_sprite_data(sprite_id, offset);
    }
}

TILE Ppu::build_tile(uint8_t x, uint8_t y, uint16_t offset){
    TILE tile = default_tile;
    uint8_t block_id = this->get_block_id(x, y);
    uint8_t sprite_id = this->get_sprite_id(x, y, offset);
    uint8_t attr = this->get_attribute(x, y, offset);
    tile.sprite_id = sprite_id;
    // dprint("%d %p", sprite_id, sprite_id);
    tile.palette_id = (attr >> (block_id * 2)) & 0x03;
    tile.sprite.data =
        this->build_sprite_data(sprite_id, this->get_background_table_offset());
    tile.scroll_x = this->scroll_x;
    tile.scroll_y = this->scroll_y;
    return tile;
}

// draw every 8 line
void Ppu::build_background(){
    uint8_t mod_y = this->get_tile_y() % V_SPRITE_NUM;
    uint8_t table_id_offset = ((this->get_tile_y() / V_SPRITE_NUM) % 2) ? 2 : 0;

    for (uint8_t x = 0; x < H_SPRITE_NUM; x++) {
        uint8_t tile_x = (x + this->get_scroll_tile_x());
        uint16_t mod_x = x % H_SPRITE_NUM;
        uint16_t name_table_id = (x / H_SPRITE_NUM) % 2 + table_id_offset;
        uint16_t offset_addr_by_name_table = name_table_id * 0x0400;
        TILE tile = this->build_tile(mod_x, mod_y, offset_addr_by_name_table);
        this->background.push_back(tile);
        // if (mod_y < 2)
        //     dprint("(%d, %d), %d, %p, addr:%d, %d",
        //         mod_x, mod_y, tile.sprite_id, tile.sprite_id, 
        //         offset_addr_by_name_table, this->background.size());
        // for (uint8_t i = 0; i < 8; i++) {
        //     for (uint8_t j = 0; j < 8; j++) {
        //         if (tile.sprite.data[i][j]>1) {
        //             printf("%d ", tile.sprite.data[i][j]);
        //         } else {
        //             printf("  ");
        //         }
        //     }
        //     printf("\n");
        // }
    }
}

IMAGE *Ppu::run(uint16_t cycle) {
    this->cycle += cycle;
    if (this->line == 0) {
        this->background.clear();
        this->build_sprites();
        // dprint("go build_sprites");
    }
    // dprint("line:%d, cycle:%d %d", this->line, this->cycle, cycle);

    if (this->cycle > CYCLE_PER_LINE) {
        this->cycle -= CYCLE_PER_LINE;
        this->line++;

        if (this->has_sprite_hit()) {
            this->set_sprite_hit();
        }

        // every {TILE_SIZE} line
        if (this->line <= H_SIZE && 
            !(this->line % TILE_SIZE) &&
            (this->line % TILE_SIZE) == 0) {
            this->build_background();
        }

        if (this->line == H_SIZE + 1) {
            this->set_vblank();
        }

        if (this->line == H_SIZE_WITH_VBLANK) {
            // dprint("build!!!!");
            this->clear_vblank();
            this->clear_sprite_hit();
            this->line = 0;
    // dprint("# %d", this->get_is_sprite_enable());
    // dprint("# %d", this->get_is_background_enable());
            // this->image.sprites = this->get_is_sprite_enable() ?
            //     &this->sprites : nullptr;
            // this->image.background = this->get_is_background_enable() ?
            //     &this->background : nullptr;
            this->image.sprites = &this->sprites;
            this->image.background = &this->background;
            this->image.palette = this->get_palette();
            // this->palette.dump_ram();
            return &image;
        }
    }
    return nullptr;
}