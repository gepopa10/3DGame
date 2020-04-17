#ifndef TINYRAYCASTER_H
#define TINYRAYCASTER_H

#include <vector>
#include <algorithm>

#include "map.h"
#include "player.h"
#include "sprite.h"
#include "framebuffer.h"
#include "textures.h"

struct GameState {
    Map map;
    Player player;
    std::vector<Sprite> monsters;
    Texture tex_walls;
    Texture tex_monst;
    void update(const double elapsed);
};

void render(FrameBuffer &fb, const GameState &gs);

#endif // TINYRAYCASTER_H
