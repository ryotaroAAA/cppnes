#include "SDLvideo.hpp"

SDLvideo::SDLvideo(const char *title, uint16_t width, uint16_t height){
    this->title = title;
    this->width = width;
    this->height = height;
    this->initialize();
    dprint("SDL video construct!");
}

void SDLvideo::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        dprint("init failed!");
        exit(1);
    }
    if (SDL_CreateWindowAndRenderer(this->width, 
                                    this->height,
                                    0, // window flag
                                    &this->window,
                                    &this->renderer) < 0) {
        dprint("init failed!");
        exit(1);
    }
    SDL_SetWindowTitle(this->window, this->title);
    SDL_GetRendererInfo(this->renderer, &this->r_info);
    this->texture = SDL_CreateTexture(this->renderer,
                                        SDL_PIXELFORMAT_RGBA8888,
                                        SDL_TEXTUREACCESS_STREAMING,
                                        this->width,
                                        this->height);
}

void SDLvideo::render(uint32_t data[V_SIZE][H_SIZE]){
    // ignore if there is no event
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        }
        // press escape-key
        if (SDL_SCANCODE_ESCAPE == event.key.keysym.scancode) {
            dprint("escape quit!");
            exit(0);
        }
    }
    this->_render(data);
    // wait for adjusting 60fps
    this->wait();
}

void SDLvideo::_render(uint32_t data[V_SIZE][H_SIZE]){
    int pitch;
    uint8_t *pixels;
    uint32_t *line;
    SDL_LockTexture(this->texture, nullptr, reinterpret_cast<void **>(&pixels), &pitch);
    for (uint16_t y = 0; y < this->height; y++) {
        line = reinterpret_cast<uint32_t *>(pixels);
        for (int x = 0; x < this->width; x++) {
            // dprint("(x, y) = (%d, %d)", x, y);
            line[x] = data[y][x];
        }
        pixels += pitch;
    }
    SDL_UnlockTexture(texture);
    SDL_Rect rect;
    rect.x = rect.y = 0;
    rect.w = this->width;
    rect.h = this->height;
    SDL_RenderCopy(renderer, this->texture, &rect, nullptr);
    SDL_RenderPresent(renderer);
}

void SDLvideo::wait(){
    if (nextTime == 0) {
        uint32_t now = SDL_GetTicks();
        nextTime = now * 6 + 100;
        fpsTime = now;
    } else {
        uint32_t now = SDL_GetTicks();
        if (now - fpsTime >= 1000) {
            printf("FPS:%d\n", fpsCnt);
            fpsTime += 1000;
            fpsCnt = 0;
        }
        fpsCnt++;
        uint32_t nowFactored = now * 6;
        if (nowFactored < nextTime) {
            SDL_Delay((nextTime - nowFactored) / 6);
        }
        nextTime += 100;
    }
}