#include "../include/common.hpp"
#include "../include/cpu.hpp"
#include "../include/ram.hpp"
#include "../include/ppu.hpp"
#include "../include/renderer.hpp"
#include "sdl/SDLvideo.hpp"
#include <unistd.h>

int main (int argc, char* argv[]) {
    try {
        Cassette cas = Cassette(argv[1]);
        Ram wram = Ram(WRAM_SIZE);
        Ram vram = Ram(VRAM_SIZE);
        Ppu ppu = Ppu(&vram, &cas);
        Cpu cpu = Cpu(&wram, &cas, &ppu);
        cpu.reset();
        cpu.reg_dump();

        Renderer renderer = Renderer();
        SDLvideo video = SDLvideo("DEDENNES", V_SIZE, H_SIZE);

        while(1) {
        // for(int i=0; i<200; i++){
            uint16_t cycle = cpu.run();
                // dprint("#");
            IMAGE *image = ppu.run(cycle * 3);
                // dprint("#");
            // continue;
            if (image != nullptr) {
                renderer.render(image);
                // printf("[%3d]", 0);
                // for (int j = 0; j < 256; j++) {
                //     printf("%d", (j / 8)%10);                    
                // }
                // printf("\n");

                // for (int i = 0; i < 240; i++) {
                //     printf("[%3x]", i);
                //     for (int j = 0; j < 256; j++) {
                //         printf("%d", renderer.get_result()[i][j] == 0);
                //     }
                //     printf("\n");
                // }
                video.render(renderer.get_result());
                // sleep(5);
                // break;
            }
        }
    } catch (std::exception &e) {
        cout << "Standard Error: " << e.what();
        cout << std::endl;
        cout.flush();
    } catch (...) {
        cout << "Unknown Error...";
        cout << std::endl;
        cout.flush();
    }
    return 0;
}