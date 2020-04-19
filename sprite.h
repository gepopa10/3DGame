#ifndef SPRITE_H
#define SPRITE_H

#include <cstdlib>

struct Sprite {
    float x, y;
    size_t tex_id;
    float player_dist;
    float speed;
    float direction;
    bool aimed;
    int life;
    bool operator < (const Sprite& s) const;
};

#endif // SPRITE_H
