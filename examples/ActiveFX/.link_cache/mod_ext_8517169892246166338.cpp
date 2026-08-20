#include <iostream>
#include <string>
#define print(x) std::cout << (x) << std::endl;
#define LINK_rect_x (((int*)link_shm)[0])
#define LINK_rect_y (((int*)link_shm)[1])
#define LINK_rect_w (((int*)link_shm)[2])
#define LINK_rect_h (((int*)link_shm)[3])
    #include <SDL2/SDL.h>

extern "C" void link_entry(void* link_shm) {


    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("SDL in Chain", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    SDL_Event e;

    while(running) {
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) running = false;
            
            if(e.type == SDL_KEYDOWN) {
                if(e.key.keysym.sym == SDLK_RIGHT) LINK_rect_x += 10;
                if(e.key.keysym.sym == SDLK_LEFT)  LINK_rect_x -= 10;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        
        SDL_FRect rect = { 
            (float)LINK_rect_x, 
            (float)LINK_rect_y, 
            (float)LINK_rect_w, 
            (float)LINK_rect_h 
        };
        
        SDL_RenderFillRectF(renderer, &rect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

}
