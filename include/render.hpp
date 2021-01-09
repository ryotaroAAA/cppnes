#ifndef PPU_H
#define PPU_H

#include "common.hpp"
#include "ram.hpp"

class Render {
    private:
        uint16_t cycle;
    public:
        Render();
        void render(image);
        void render_background();
        void render_tile();
};

#endif //PPU_H