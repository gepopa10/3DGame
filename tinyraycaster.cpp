#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <cassert>

#include "utils.h"
#include "tinyraycaster.h"

int wall_x_texcoord(const float hitx, const float hity, const Texture &tex_walls) {
    float x = hitx - floor(hitx+.5); // x and y contain (signed) fractional parts of hitx and hity,
    float y = hity - floor(hity+.5); // they vary between -0.5 and +0.5, and one of them is supposed to be very close to 0
    int tex = x*tex_walls.size_x;
    if (std::abs(y)>std::abs(x)) // we need to determine whether we hit a "vertical" or a "horizontal" wall (w.r.t the map)
        tex = y*tex_walls.size_x;
    if (tex<0) // do not forget x_texcoord can be negative, fix that
        tex += tex_walls.size_x;
    assert(tex>=0 && tex<(int)tex_walls.size_x);
    return tex;
}

void draw_map(FrameBuffer &fb, const std::vector<std::shared_ptr<Sprite>> sprites, const Texture &tex_walls,
              const Map &map, const size_t cell_w, const size_t cell_h) {
    for (size_t j=0; j<map.h; j++) {  // draw the map itself
        for (size_t i=0; i<map.w; i++) {
            if (map.is_empty(i, j)) continue; // skip empty spaces
            size_t rect_x = i*cell_w;
            size_t rect_y = j*cell_h;
            size_t texid = map.get(i, j);
            assert(texid<tex_walls.count);
            fb.draw_rectangle(rect_x, rect_y, cell_w, cell_h, tex_walls.get(0, 0, texid)); // the color is taken from the upper left pixel of the texture #texid
        }
    }
    for (size_t i=0; i<sprites.size(); i++) { // show the monsters
        fb.draw_rectangle(sprites[i]->x*cell_w-3, sprites[i]->y*cell_h-3, 6, 6, pack_color(255, 0, 0));
    }
}

void GameState::update(const double elapsed) {

  //updating the position of player and monsters
  player.a += float(player.turn)*elapsed;
  float nx = player.x + player.walk*cos(player.a)*elapsed*player.speed*2;
  float ny = player.y + player.walk*sin(player.a)*elapsed*player.speed*2;

  bool oneMsgSaid = false;
  //check if the map is empty at the new position before updating
  if (int(nx)>=0 && int(nx)<int(map.w) && int(ny)>=0 && int(ny)<int(map.h)) {
      if (map.is_empty(nx, player.y)) player.x = nx;
      if (map.is_empty(player.x, ny)) player.y = ny;
  }

  for (std::shared_ptr<Sprite> &sprite : sprites) { // loop on all sprites

    sprite->player_dist = std::sqrt(pow(player.x - sprite->x, 2) + pow(player.y - sprite->y, 2));

    //dealing with movable monsters
    std::shared_ptr<Monster> maybeMonster = std::dynamic_pointer_cast<Monster>(sprite); //we use dynamic_pointer_cast because Monster is a shared_ptr
    if (maybeMonster != nullptr){ // Cast succeeded, the sprite is a monster
        maybeMonster->update(map, player, elapsed); // make the monsters advance in the players direction and other update
        //set the pointer to null if the monster is dead so it gets erased after
        if (maybeMonster->life <= 0) {maybeMonster->state = Monster::dead_state; maybeMonster->manageDead(sprite);}
        //we set the msg and said that at least one mob said a msg so dont put empty msg.
        if (strcmp(maybeMonster->msg(),"")) {oneMsgSaid=true; gameplayMessage = maybeMonster->msg();}
    }
  }

  if(!oneMsgSaid) {gameplayMessage = ""; oneMsgSaid = false;} //resetting only if no mob is saying anything, helps to not override other mobs msgs

  sprites.erase(std::remove(begin(sprites), end(sprites), nullptr), end(sprites)); //remove pointers in vector that are null (dead monsters)

  // sort it from farthest to closest
  std::sort(sprites.begin(), sprites.end(),
           [](std::shared_ptr<Sprite>& x, std::shared_ptr<Sprite>& y)
             {return x->player_dist > y->player_dist;}); // shared_ptr implements the “<“ operator
                                                         // but, it doesn’t invoke “<“ on the pointed-to objects
                                                         // we have to provide sort with a comparison function (as lambda function here)
                                                         // and we cannot overload < operator in Sprite class
}

void render(FrameBuffer &fb, GameState &gs) { // not a member of GameState struct
    const Map &map                                      = gs.map;
    Player &player                                      = gs.player;
    const std::vector<std::shared_ptr<Sprite>> sprites  = gs.sprites;
    const Texture &tex_walls                            = gs.tex_walls;

    fb.clear(pack_color(40, 40, 40)); // clear the screen
    fb.set_floor(pack_color(110, 110, 110)); // clear the screen

    const size_t cell_w = fb.w/(map.w); // size of one map cell on the screen
    const size_t cell_h = fb.h/map.h;
    std::vector<float> depth_buffer(fb.w, 1e3);

    for (size_t i=0; i<fb.w; i++) { // draw the "3D" view
        float angle = player.a-player.fov/2 + player.fov*i/float(fb.w);
        for (float t=0; t<50; t+=.01) { // ray marching loop
            float x = player.x + t*cos(angle);
            float y = player.y + t*sin(angle);

            if (map.is_empty(x, y)) continue;

            size_t texid = map.get(x, y); // our ray touches a wall, so draw the vertical column to create an illusion of 3D
            assert(texid<tex_walls.count);
            float dist = t*cos(angle-player.a);
            depth_buffer[i] = dist;
            size_t column_height = std::min(2000, int(fb.h/dist));
            int x_texcoord = wall_x_texcoord(x, y, tex_walls);
            std::vector<uint32_t> column = tex_walls.get_scaled_column(texid, x_texcoord, column_height);
            int pix_x = i ;
            for (size_t j=0; j<column_height; j++) { // copy the texture column to the framebuffer
                int pix_y = j + fb.h/2 - column_height/2;
                if (pix_y>=0 && pix_y<(int)fb.h) {
                    fb.set_pixel(pix_x, pix_y, column[j]);
                }
            }
            break;
        }
    } // field of view ray sweeping

    for (size_t i=0; i<sprites.size(); i++) { // draw the sprites
        sprites[i]->draw_sprite(fb, depth_buffer, player);
    }

    player.render(fb);

    //we clear the screen and draw only the map if in map mode is active
    if (gs.mapModeActive){
      fb.clear(pack_color(255, 255, 255)); // clear the screen
      for (size_t i=0; i<fb.w; i++) { // draw the visibility cone
          float angle = player.a-player.fov/2 + player.fov*i/float(fb.w);
          for (float t=0; t<50; t+=.01) { // ray marching loop
              float x = player.x + t*cos(angle);
              float y = player.y + t*sin(angle);

              fb.set_pixel(x*cell_w, y*cell_h, pack_color(190, 190, 190)); // this draws the visibility cone

              if (map.is_empty(x, y)) continue;
              break;
            }
        }
      draw_map(fb, sprites, tex_walls, map, cell_w, cell_h);
    }

    if (gs.player.life <=0){
      fb.clear(pack_color(0, 0, 0));
      gs.gameplayMessage = "GAME OVER";
    }
}
