#ifndef SPRITE_H
#define SPRITE_H

#include <cstdlib>
#include <chrono>

struct Sprite {
    float x, y;
    size_t tex_id;
    float player_dist;
    float speed;
    float direction;
    bool aimed;
    int life;
    std::chrono::duration<double,  std::ratio<1>> timefromLastAttack_secs;
    std::chrono::time_point<std::chrono::high_resolution_clock> timeatLastAttack_secs;
    bool operator < (const Sprite& s) const;
};

#endif // SPRITE_H
