/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2021 Jorengarenar <dev@joren.ga>
 * Copyright (C) 2005 Ingo Ruhnke  <grumbel@gmx.de>
 */

#include <math.h>
#include <stdlib.h>

#include <gba_input.h>
#include <gba_interrupt.h>
#include <gba_video.h>
#include <fade.h>

#include "console.hpp"
#include "globals.hpp"
#include "math.hpp"
#include "sprite_manager.hpp"
#include "sprite_renderer.hpp"
#include "tile_renderer.hpp"
#include "tileset.hpp"
#include "tux.hpp"

#include "level.hpp"

// Data for use in the game
#include "../build/level1_bin.h"
#include "../build/antarctica.h"

#include "../build/sprites.h"

unsigned int frame;

void VblankInterrupt()
{
    frame += 1;
}

void vid_vsync()
{
    while (REG_VCOUNT >= 160); // wait till VDraw
    while (REG_VCOUNT <  160); // wait till VBlank
}

int main(void)
{
    // Set up the interrupt handlers
    irqInit();

    // irqSet( Int_Vblank, VblankInterrupt);

    // Enable Vblank Interrupt to allow VblankIntrWait
    // irqEnable(Int_Vblank);

    // Allow Interrupts
    REG_IME = 1;

    SetMode( MODE_0 | BG0_ON | BG1_ON | BG2_ON | BG3_ON | OBJ_ON ); // screen mode & background to display

    // console layer
    BGCTRL[0] = BG_PRIORITY(0) | CHAR_BASE(0) | BG_MOSAIC | BG_256_COLOR | SCREEN_BASE(28) | BG_SIZE_0;
    BGCTRL[1] = BG_PRIORITY(0) | CHAR_BASE(0) | BG_MOSAIC | BG_256_COLOR | SCREEN_BASE(29) | BG_SIZE_0;
    BGCTRL[2] = BG_PRIORITY(0) | CHAR_BASE(0) | BG_MOSAIC | BG_256_COLOR | SCREEN_BASE(30) | BG_SIZE_0;
    BGCTRL[3] = BG_PRIORITY(0) | CHAR_BASE(0) | BG_MOSAIC | BG_256_COLOR | SCREEN_BASE(31) | BG_SIZE_0;

    TileManager stack_tile_manager;
    TileRenderer stack_tile_renderer;

    SpriteManager stack_sprite_manager;
    SpriteRenderer stack_sprite_renderer;

    tile_manager  = &stack_tile_manager;
    tile_renderer = &stack_tile_renderer;

    sprite_renderer = &stack_sprite_renderer;
    sprite_manager  = &stack_sprite_manager;


    Level level1{ (uint16_t*)level1_bin, antarcticaPal, antarcticaTiles };
    level = &level1;

    sprite_renderer->set_palette(spritesPal);
    sprite_renderer->upload((uint16_t*)spritesTiles);

    console.print("SuperTux Advance  v0.0.0\n");

    Tux tux;

    while (1) {
        tux.update();
        sprite_renderer->update();
        scanKeys();
        // VBlankIntrWait();
        vid_vsync();
    }
}
