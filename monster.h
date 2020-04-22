#ifndef MONSTER_H
#define MONSTER_H

#include "sprite.h"
#include "map.h"

struct Monster : Sprite {

    float speed;
    float direction;
    bool aimed;
    int life;
    std::chrono::duration<double,  std::ratio<1>> timefromLastAttack_secs;
    std::chrono::time_point<std::chrono::high_resolution_clock> timeatLastAttack_secs;

    Monster(float x_in,
            float y_in,
            Texture texture_in,
            size_t tex_id_in = 0,
            float speed_in = 0,
            float player_dist_in = 10000,
            float direction_in = 0,
            bool aimed_in = 0,
            int life_in = 0);

    virtual void draw_sprite(FrameBuffer &fb,
                             const std::vector<float> &depth_buffer,
                             const Player &player);

    virtual void updatePosition(const Map &map, Player &player, const double elapsed);
    

};

#endif // MONSTER_H
