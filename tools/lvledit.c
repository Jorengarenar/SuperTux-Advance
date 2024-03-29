/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2021 Jorengarenar <dev@joren.ga>
 */

#include <stdio.h>
#include <stdint.h>

#include <SDL2/SDL.h>

#define SCALE 3

int main()
{
    FILE* f = fopen("../data/levels/level1.bin", "rb");

    uint16_t colmap_size = 1024;
    fread(&colmap_size, sizeof colmap_size, 1, f);
    uint8_t* colmap = malloc(colmap_size * (sizeof *colmap));
    fread(colmap, sizeof *colmap, colmap_size, f);

    uint16_t width;
    uint16_t height;
    fread(&width,  sizeof width,  1, f);
    fread(&height, sizeof height, 1, f);
    uint16_t* interactive = malloc(width*height * (sizeof *interactive));
    fread(interactive, sizeof *interactive, width*height, f);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_CreateWindowAndRenderer(SCALE*width, SCALE*height, 0, &window, &renderer);

    SDL_RenderSetScale(renderer, SCALE, SCALE);

    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            uint16_t hexValue = colmap[interactive[i*width + j]];
            /* uint16_t hexValue = interactive[i*width + j]; */

            if (hexValue) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }

            SDL_RenderDrawPoint(renderer, j, i);
        }
    }
    SDL_RenderPresent(renderer);

    while (1) {
        SDL_Event event;
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
            break;
        }
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    free(colmap);
    free(interactive);

    return 0;
}

// vim: fdl=1
