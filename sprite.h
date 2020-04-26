#ifndef SPRITE_H
#define SPRITE_H

#include <cstdlib>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <memory>

#include "player.h"
#include "textures.h"
#include "framebuffer.h"

struct Sprite {
    float x, y;
    Texture texture;
    size_t tex_id;
    float player_dist;

    Sprite(float x_in,
           float y_in,
           Texture texture_in,
           size_t tex_id_in = 0,
           float player_dist_in = 10000);

    virtual void draw_sprite(FrameBuffer &fb,
                             const std::vector<float> &depth_buffer,
                             const Player &player);

};

#endif // SPRITE_H
