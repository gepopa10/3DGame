#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <cassert>

#include "utils.h"
#include "tinyraycaster.h"

int wall_x_texcoord(const float hitx, const float hity, const Texture &tex_walls) {
    float x = hitx - floor(hitx+.5); // x and y contain (signed) fractional parts of hitx and hity,
    float y = hity - floor(hity+.5); // they vary between -0.5 and +0.5, and one of them is supposed to be very close to 0
    int tex = x*tex_walls.size;
    if (std::abs(y)>std::abs(x)) // we need to determine whether we hit a "vertical" or a "horizontal" wall (w.r.t the map)
        tex = y*tex_walls.size;
    if (tex<0) // do not forget x_texcoord can be negative, fix that
        tex += tex_walls.size;
    assert(tex>=0 && tex<(int)tex_walls.size);
    return tex;
}

void draw_map(FrameBuffer &fb, const std::vector<Sprite> &sprites, const Texture &tex_walls, const Map &map, const size_t cell_w, const size_t cell_h) {
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
        fb.draw_rectangle(sprites[i].x*cell_w-3, sprites[i].y*cell_h-3, 6, 6, pack_color(255, 0, 0));
    }
}

void draw_sprite(FrameBuffer &fb, Sprite &sprite, const std::vector<float> &depth_buffer, const Player &player, const Texture &tex_sprites) {
    // absolute direction from the player to the sprite (in radians)
    float sprite_dir = atan2(sprite.y - player.y, sprite.x - player.x);
    while (sprite_dir - player.a >  M_PI) sprite_dir -= 2*M_PI; // remove unncesessary periods from the relative direction
    while (sprite_dir - player.a < -M_PI) sprite_dir += 2*M_PI;

    size_t sprite_screen_size = std::min(2000, static_cast<int>(fb.h/sprite.player_dist)); // screen sprite size
    int h_offset = (sprite_dir - player.a)*(fb.w/2)/(player.fov) + (fb.w/2)/2 - sprite_screen_size/2; // do not forget the 3D view takes only a half of the framebuffer, thus fb.w/2 for the screen width
    int v_offset = fb.h/2 - sprite_screen_size/2;

    sprite.aimed = false;
    for (size_t i=0; i<sprite_screen_size; i++) {
        if (h_offset+int(i)<0 || h_offset+i>=fb.w/2) continue;
        if (depth_buffer[h_offset+i]<sprite.player_dist) continue; // this sprite column is occluded
        for (size_t j=0; j<sprite_screen_size; j++) {
            if (v_offset+int(j)<0 || v_offset+j>=fb.h) continue;
            uint32_t color = tex_sprites.get(i*tex_sprites.size/sprite_screen_size, j*tex_sprites.size/sprite_screen_size, sprite.tex_id);
            uint8_t r,g,b,a;
            unpack_color(color, r, g, b, a);
            if (a>128){
              uint8_t r_set = (r-255)*sprite.life/100.0 + 255; //linear function
              uint32_t color = pack_color(r_set, g, b); //change the color of the sprite depending on his life
              fb.set_pixel(fb.w/2 + h_offset+i, v_offset+j, color);
            }
            if (fb.w/2 + h_offset+i == fb.w/2 + fb.w/4) sprite.aimed = true; //if a single pixel of the sprite is in the center of the view we aimed it
        }
    }
}

void draw_visor(FrameBuffer &fb) {
  // params of the players visor
  size_t visorWidth = 16; //in pixels
  size_t visorHeight = 16;
  size_t visorThickness = 2;
  size_t centerEmpty = 4;
  uint32_t visorColor = pack_color(255, 0, 0); //color of the visor

  fb.draw_rectangle(fb.w/2 + fb.w/4 - visorWidth/2, fb.h/2 - visorThickness/2, visorWidth/2 - centerEmpty/2, visorThickness, visorColor);
  fb.draw_rectangle(fb.w/2 + fb.w/4 + centerEmpty/2, fb.h/2 - visorThickness/2, visorWidth/2 - centerEmpty/2, visorThickness, visorColor);
  fb.draw_rectangle(fb.w/2 + fb.w/4 - visorThickness/2, fb.h/2 - visorHeight/2, visorThickness, visorHeight/2 -centerEmpty/2, visorColor);
  fb.draw_rectangle(fb.w/2 + fb.w/4 - visorThickness/2, fb.h/2 + centerEmpty/2, visorThickness, visorHeight/2 -centerEmpty/2, visorColor);
}

void GameState::update(const double elapsed,
                       const FrameBuffer& fb) {

  //updating the position of player and monsters
  player.a += float(player.turn)*elapsed;
  float nx = player.x + player.walk*cos(player.a)*elapsed*player.speed;
  float ny = player.y + player.walk*sin(player.a)*elapsed*player.speed;

  //check if the map is empty at the new position before updating
  if (int(nx)>=0 && int(nx)<int(map.w) && int(ny)>=0 && int(ny)<int(map.h)) {
      if (map.is_empty(nx, player.y)) player.x = nx;
      if (map.is_empty(player.x, ny)) player.y = ny;
  }

  for (size_t i=0; i<monsters.size(); i++) { // make the monsters advance in the players direction
    float sprite_dir = atan2(monsters[i].y - player.y, monsters[i].x - player.x);
    while (sprite_dir - player.a >  M_PI) sprite_dir -= 2*M_PI; // remove unncesessary periods from the relative direction
    while (sprite_dir - player.a < -M_PI) sprite_dir += 2*M_PI;
    monsters[i].player_dist = std::sqrt(pow(player.x - monsters[i].x, 2) + pow(player.y - monsters[i].y, 2));

    size_t sprite_screen_size = std::min(2000, static_cast<int>(fb.h/monsters[i].player_dist)); // screen sprite size in pixels
    //enabling firing in the direction of the monster
    if (player.fire){
      for (float t=0; t<20; t+=.01) { // ray marching loop in players view angle
          float x = player.x + t*cos(player.a);
          float y = player.y + t*sin(player.a);
          if (!map.is_empty(x, y)){
            float dist = std::sqrt(pow(player.x - x, 2) + pow(player.y - y, 2)); ///dist to wall in map size (16)
            float distShoot = 8.0; //in wall size 16 usually
            size_t weapongDmgs = 25;
            if (monsters[i].player_dist < fabs(dist) //check if the monsters is not behind a wall
                && monsters[i].player_dist < distShoot //check if the monster is close enough to be shoot
                && monsters[i].aimed){ //check if we aimed at monster (is set in draw_sprite in render)
              monsters[i].life -= weapongDmgs;
            }
            break;
          }
        }
    }

    //update position of monsters
    float proximityAttackThreshold = 4.0; //If the player is closer than 4 mapcells from a monster it starts attacking
    float proximityToPlayer = 0.5; //If the player is closer than 1 mapcells from a monster, the monster doesnt go further (to avoid entering in the player)
    if (monsters[i].player_dist < proximityAttackThreshold) monsters[i].direction = sprite_dir; monsters[i].speed = 0.5;

    float nx_sprite = monsters[i].x - cos(monsters[i].direction)*elapsed*monsters[i].speed;
    float ny_sprite = monsters[i].y - sin(monsters[i].direction)*elapsed*monsters[i].speed;

    if (int(nx_sprite)>=0 && int(nx_sprite)<int(map.w) && int(ny_sprite)>=0
        && int(ny_sprite)<int(map.h) && monsters[i].player_dist > proximityToPlayer) {
        if (map.is_empty(nx_sprite, ny_sprite)) { //new position is free of walls
          monsters[i].x = nx_sprite; monsters[i].y = ny_sprite;
        } else {//hitting a wall
          monsters[i].direction += M_PI + getRandom(-M_PI/6, M_PI/6); //turn around + small random angle
        }
    }

    monsters[i].player_dist = std::sqrt(pow(player.x - monsters[i].x, 2) + pow(player.y - monsters[i].y, 2)); //updating with new dist to sort

    //enable monster to attack player
    float distanceMonsterAttack = 1; //if monster is closer to distanceMonsterAttack it can inflict damage to player
    int dmgMonsterAttack = 10;
    float timeAttackMonster = 1; //minimum time before monster can reattack
    monsters[i].timefromLastAttack_secs =  std::chrono::high_resolution_clock::now() - monsters[i].timeatLastAttack_secs; //time from last attack
    if (monsters[i].player_dist < distanceMonsterAttack && monsters[i].timefromLastAttack_secs.count() > timeAttackMonster) {
      player.life -= dmgMonsterAttack;
      monsters[i].timeatLastAttack_secs = std::chrono::high_resolution_clock::now(); //reseting time at attack
    }

  } //end loop on monsters
  std::cout << "player.life " << player.life << std::endl;

  //cleaning monsters who are dead
  monsters.erase(std::remove_if(monsters.begin(), monsters.end(),[](const Sprite& x) {return x.life <= 0;}), monsters.end());

  std::sort(monsters.begin(), monsters.end()); // sort it from farthest to closest
}

void render(FrameBuffer &fb, GameState &gs) {
    const Map &map                     = gs.map;
    const Player &player               = gs.player;
    std::vector<Sprite> &sprites = gs.monsters;
    const Texture &tex_walls           = gs.tex_walls;
    const Texture &tex_monst           = gs.tex_monst;

    fb.clear(pack_color(255, 255, 255)); // clear the screen

    const size_t cell_w = fb.w/(map.w*2); // size of one map cell on the screen
    const size_t cell_h = fb.h/map.h;
    std::vector<float> depth_buffer(fb.w/2, 1e3);

    for (size_t i=0; i<fb.w/2; i++) { // draw the visibility cone AND the "3D" view
        float angle = player.a-player.fov/2 + player.fov*i/float(fb.w/2);
        for (float t=0; t<20; t+=.01) { // ray marching loop
            float x = player.x + t*cos(angle);
            float y = player.y + t*sin(angle);
            fb.set_pixel(x*cell_w, y*cell_h, pack_color(190, 190, 190)); // this draws the visibility cone

            if (map.is_empty(x, y)) continue;

            size_t texid = map.get(x, y); // our ray touches a wall, so draw the vertical column to create an illusion of 3D
            assert(texid<tex_walls.count);
            float dist = t*cos(angle-player.a);
            depth_buffer[i] = dist;
            size_t column_height = std::min(2000, int(fb.h/dist));
            int x_texcoord = wall_x_texcoord(x, y, tex_walls);
            std::vector<uint32_t> column = tex_walls.get_scaled_column(texid, x_texcoord, column_height);
            int pix_x = i + fb.w/2; // we are drawing at the right half of the screen, thus +fb.w/2
            for (size_t j=0; j<column_height; j++) { // copy the texture column to the framebuffer
                int pix_y = j + fb.h/2 - column_height/2;
                if (pix_y>=0 && pix_y<(int)fb.h) {
                    fb.set_pixel(pix_x, pix_y, column[j]);
                }
            }
            break;
        } // ray marching loop
    } // field of view ray sweeping

    draw_map(fb, sprites, tex_walls, map, cell_w, cell_h);

    for (size_t i=0; i<sprites.size(); i++) { // draw the sprites
        draw_sprite(fb, sprites[i], depth_buffer, player, tex_monst);
    }

    draw_visor(fb);
}
