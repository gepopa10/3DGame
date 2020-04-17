#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include <cmath>

#include "utils.h"
#include "tinyraycaster.h"
#include "window.h"

int main() {

    FrameBuffer fb{1024, 512, std::vector<uint32_t>(1024*512, pack_color(255, 255, 255))};
    GameState gs{ Map(),                                // game map
                  {3.456, 2.345, 1.523, M_PI/3., 0, 0}, // player
                  { {3.523, 3.812, 2, 0},               // monsters lists
                    {1.834, 8.765, 0, 0},
                    {5.323, 5.365, 1, 0},
                    {14.32, 13.36, 3, 0},
                    {4.123, 10.76, 1, 0} },
                  Texture("../walltext.bmp", SDL_PIXELFORMAT_ABGR8888),  // textures for the walls
                  Texture("../monsters.bmp", SDL_PIXELFORMAT_ABGR8888)}; // textures for the monsters
    if (!gs.tex_walls.count || !gs.tex_monst.count) {
        std::cerr << "Failed to load textures" << std::endl;
        return -1;
    }

    Window guiWindow(fb);

    auto t1 = std::chrono::high_resolution_clock::now();
    while (1) {
        { // sleep if less than 20 ms since last re-rendering; TODO: decouple rendering and event polling frequencies
            auto t2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
            if (fp_ms.count()<20) {
                std::this_thread::sleep_for(std::chrono::milliseconds(2));
                continue;
            }
            t1 = t2;
        }

        { // poll events and update player's state (walk/turn flags); TODO: move this block to a more appropriate place
            SDL_Event event;
            if (SDL_PollEvent(&event)) {
                if (SDL_QUIT==event.type || (SDL_KEYDOWN==event.type && SDLK_ESCAPE==event.key.keysym.sym)) break;
                if (SDL_KEYUP==event.type) { //SDL_KEYUP when user releases button
                    if ('a'==event.key.keysym.sym || 'd'==event.key.keysym.sym) gs.player.turn = 0;
                    if ('w'==event.key.keysym.sym || 's'==event.key.keysym.sym) gs.player.walk = 0;
                }
                if (SDL_KEYDOWN==event.type) { //SDL_KEYDOWN when user presses
                    if ('a'==event.key.keysym.sym) gs.player.turn = -1;
                    if ('d'==event.key.keysym.sym) gs.player.turn =  1;
                    if ('w'==event.key.keysym.sym) gs.player.walk =  1;
                    if ('s'==event.key.keysym.sym) gs.player.walk = -1;
                }
            }
        }

        { // update player's position; TODO: move this block to a more appropriate place
            gs.player.a += float(gs.player.turn)*.05; // TODO measure elapsed time and modify the speed accordingly
            float nx = gs.player.x + gs.player.walk*cos(gs.player.a)*.05;
            float ny = gs.player.y + gs.player.walk*sin(gs.player.a)*.05;

            //check if the map is empty at the new position before updating
            if (int(nx)>=0 && int(nx)<int(gs.map.w) && int(ny)>=0 && int(ny)<int(gs.map.h)) {
                if (gs.map.is_empty(nx, gs.player.y)) gs.player.x = nx;
                if (gs.map.is_empty(gs.player.x, ny)) gs.player.y = ny;
            }

            for (size_t i=0; i<gs.monsters.size(); i++) { // make the monsters advance in the players direction
              gs.monsters[i].player_dist = std::sqrt(pow(gs.player.x - gs.monsters[i].x, 2) + pow(gs.player.y - gs.monsters[i].y, 2));
              float sprite_dir = atan2(gs.monsters[i].y - gs.player.y, gs.monsters[i].x - gs.player.x);
              float nx_sprite = gs.monsters[i].x - gs.monsters[i].player_dist*cos(sprite_dir)*.01;
              float ny_sprite = gs.monsters[i].y - gs.monsters[i].player_dist*sin(sprite_dir)*.01;
              if (int(nx_sprite)>=0 && int(nx_sprite)<int(gs.map.w) && int(ny_sprite)>=0 && int(ny_sprite)<int(gs.map.h)) {
                  if (gs.map.is_empty(nx_sprite, gs.monsters[i].y)) gs.monsters[i].x = nx_sprite;
                  if (gs.map.is_empty(gs.monsters[i].x, ny_sprite)) gs.monsters[i].y = ny_sprite;
              }
              gs.monsters[i].player_dist = std::sqrt(pow(gs.player.x - gs.monsters[i].x, 2) + pow(gs.player.y - gs.monsters[i].y, 2));
            }

            // for (size_t i=0; i<gs.monsters.size(); i++) { // update the distances from the player to each sprite
            //     gs.monsters[i].player_dist = std::sqrt(pow(gs.player.x - gs.monsters[i].x, 2) + pow(gs.player.y - gs.monsters[i].y, 2));
            // }
            std::sort(gs.monsters.begin(), gs.monsters.end()); // sort it from farthest to closest
          }

        render(fb, gs); // render the scene to the frambuffer

        guiWindow.display();
    }

    return 0;
}
