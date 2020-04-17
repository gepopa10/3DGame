#ifndef PLAYER_H
#define PLAYER_H

struct Player {
    float x, y; // position
    float a;    // view direction
    float fov;  // field of view
    int turn, walk ;
    float speed;
};

#endif // PLAYER_H
