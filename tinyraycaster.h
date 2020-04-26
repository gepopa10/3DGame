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
#include "hitler.h"
#include "framebuffer.h"
#include "textures.h"

struct GameState {
    GameState(Map map_in, Player player_in, std::vector<std::shared_ptr<Sprite>> sprites_in, Texture tex_walls_in, const char* gameplayMessage_in = "", const char* gameName_in = "Gemuseholle", bool mapModeActive_in = false):
              map(map_in), player(player_in), sprites(sprites_in), tex_walls(tex_walls_in), gameplayMessage(gameplayMessage_in), gameName(gameName_in), mapModeActive(mapModeActive_in){};


    Map map;
    Player player;
    std::vector<std::shared_ptr<Sprite>> sprites;
    Texture tex_walls;
    const char* gameplayMessage;
    const char* gameName;
    bool mapModeActive;
    void update(const double elapsed);

};

void render(FrameBuffer &fb, GameState &gs);

#endif // TINYRAYCASTER_H
