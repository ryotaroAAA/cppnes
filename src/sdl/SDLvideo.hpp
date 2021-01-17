#ifndef SDLVIDEO_H
#define SDLVIDEO_H

#include "../../include/common.hpp"
#include "../../include/ppu.hpp"
#include "../../include/ram.hpp"
#include "../../external/SDL2/include/SDL.h"

class SDLvideo {
    private:
        const char *title;
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;
        SDL_RendererInfo r_info;
        uint16_t width;
        uint16_t height;
        bool isFullscreen;
        uint32_t nextTime;
        uint32_t fpsTime;
        uint32_t fpsCnt;
        void initialize();
        void _render(uint32_t data[V_SIZE][H_SIZE]);
        void wait();
    public:
        SDLvideo(const char *title, uint16_t width, uint16_t height);
        void render(uint32_t data[V_SIZE][H_SIZE]);
};

#endif //SDLVIDEO_H