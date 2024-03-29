/* SPDX-License-Identifier: GPL-2.0-or-later
 * Copyright (C) 2021 Jorengarenar <dev@joren.ga>
 * Copyright (C) 2005 Ingo Ruhnke  <grumbel@gmx.de>
 */

#include "tile_map.hpp"

#include "tileset.hpp"

TileMap::TileMap(uint16_t* raw_data) :
    width(raw_data[0]),
    height(raw_data[1]),
    tilemap(raw_data + 2)
{}

TileMap::~TileMap() {}
