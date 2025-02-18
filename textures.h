#ifndef TEXTURES_H
#define TEXTURES_H

#include <vector>
#include <cstdint>
#include <iostream>
#include <cassert>
#include <math.h>

#include "SDL.h"
#include <SDL_image.h>

#include "utils.h"

struct Texture {
    size_t img_w, img_h; // overall image dimensions
    size_t count, size_x, size_y;  // number of textures and sizes in pixels
    std::vector<uint32_t> img; // textures storage container

    Texture(const std::string filename, const uint32_t format, const int cols = 1, const int rows = 1);
    uint32_t get(const size_t i, const size_t j, const size_t idx) const; // get the pixel (i,j) from the texture idx
    std::vector<uint32_t> get_scaled_column(const size_t texture_id, const size_t tex_coord, const size_t column_height) const; // retrieve one column (tex_coord) from the texture texture_id and scale it to the destination size
};

#endif // TEXTURES_H
