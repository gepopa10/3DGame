#ifndef MONSTERANIMATED_H
#define MONSTERANIMATED_H

#include "monster.h"

struct MonsterAnimated : Monster {

    float directionIni; //initial direction in which the monster is
    float distanceSameDirection; //travel in same direction count
    const int distanceWall = 1; //has to keep this position from a wall
    const float maxDistanceSameDirection = 3; //max distance monster can travel in same direction
    bool firstMotion = true; //to start timer at the very first motion

    // using Monster::Monster; //inheriting constructor
    MonsterAnimated(float x_in,
                    float y_in,
                    Texture texture_in,
                    size_t tex_id_in = 0,
                    float player_dist_in = 10000,
                    float speed_in = 0.5,
                    float direction_in = 0,
                    bool aimed_in = 0,
                    int life_in = 100);

    virtual uint8_t changeColorLife(const uint8_t r) override {return r;};

};

#endif // MONSTERANIMATED_H
