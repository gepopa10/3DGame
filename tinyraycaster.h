#ifndef TINYRAYCASTER_H
#define TINYRAYCASTER_H

#include <vector>
#include <algorithm>
#include <iomanip>
#include <memory>
#include <functional>

#include "SDL.h"

#include "map.h"
#include "player.h"
#include "sprite.h"
#include "monster.h"
#include "monsteranimated.h"
#include "boss.h"
#include "guard.h"
#include "framebuffer.h"
#include "textures.h"

struct GameState {
    Map map;
    Player player;
    std::vector<std::shared_ptr<Sprite>> sprites;
    Texture tex_walls;
    void update(const double elapsed);
};

void render(FrameBuffer &fb, GameState &gs);

#endif // TINYRAYCASTER_H
