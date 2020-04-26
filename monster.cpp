#include "monster.h"

Monster::Monster(float x_in,
                 float y_in,
                 Texture texture_in,
                 size_t tex_id_in,
                 float player_dist_in,
                 float speed_in,
                 float direction_in,
                 bool aimed_in,
                 int life_in):
                 Sprite(x_in, y_in, texture_in, tex_id_in, player_dist_in),
                 speed(speed_in),
                 direction(direction_in),
                 aimed(aimed_in),
                 life(life_in),
                 msgTime_secs(0),
                 msgTimeLimit_secs(5){

}

void Monster::draw_sprite(FrameBuffer &fb,
                          const std::vector<float> &depth_buffer,
                          const Player &player) {

    // absolute direction from the player to the sprite (in radians)
    float sprite_dir = atan2(y - player.y, x - player.x);
    while (sprite_dir - player.a >  M_PI) sprite_dir -= 2*M_PI; // remove unncesessary periods from the relative direction
    while (sprite_dir - player.a < -M_PI) sprite_dir += 2*M_PI;

    size_t sprite_screen_size = std::min(2000, static_cast<int>(fb.h/player_dist)); // screen sprite size
    int h_offset = (sprite_dir - player.a)*(fb.w/2)/(player.fov) + (fb.w/2)/2 - sprite_screen_size/2; // do not forget the 3D view takes only a half of the framebuffer, thus fb.w/2 for the screen width
    int v_offset = fb.h/2 - sprite_screen_size/2;

    aimed = false;
    for (size_t i=0; i<sprite_screen_size; i++) {
        if (h_offset+int(i)<0 || h_offset+i>=fb.w/2) continue;
        if (depth_buffer[h_offset+i]<player_dist) continue; // this sprite column is occluded
        for (size_t j=0; j<sprite_screen_size; j++) {
            if (v_offset+int(j)<0 || v_offset+j>=fb.h) continue;
            uint32_t color = texture.get(i*texture.size_x/sprite_screen_size, j*texture.size_y/sprite_screen_size, tex_id);
            uint8_t r,g,b,a;
            unpack_color(color, r, g, b, a);
            if (a>128){
              uint32_t color = pack_color(changeColorLife(r), g, b); //change the color of the sprite depending on his life
              fb.set_pixel(fb.w/2 + h_offset+i, v_offset+j, color);
            }
            if (fb.w/2 + h_offset+i == fb.w/2 + fb.w/4) aimed = true; //if a single pixel of the sprite is in the center of the view we aimed it
        }
    }
}

void Monster::update(const Map &map, Player &player, const double elapsed){

  //check and updated if the player attacks monster
  attacked(map, player);
  //change the state depending on the proximity of the monster to player
  checkandUpdateState(map, player);
  //take action based on state of the monster
  action(map, player, elapsed);
}

void Monster::attacked(const Map &map, const Player &player){
  if (player.fire){
    for (float t=0; t<20; t+=.01) { // ray marching loop in players view angle
        float x = player.x + t*cos(player.a);
        float y = player.y + t*sin(player.a);
        if (!map.is_empty(x, y)){
          float dist = std::sqrt(pow(player.x - x, 2) + pow(player.y - y, 2)); ///dist to wall in map size (16)
          if (player_dist < fabs(dist) //check if the monsters is not behind a wall
              && player_dist < player.currentWeapon.distShoot //check if the monster is close enough to be shoot
              && aimed){ //check if we aimed at monster (is set in draw_sprite in render)
            life -= player.currentWeapon.weapongDmgs;
          }
          break;
        }
      }
  }
}

void Monster::checkandUpdateState(const Map &map, const Player &player){
  float sprite_dir = atan2(y - player.y, x - player.x);
  while (sprite_dir - player.a >  M_PI) sprite_dir -= 2*M_PI; // remove unncesessary periods from the relative direction
  while (sprite_dir - player.a < -M_PI) sprite_dir += 2*M_PI;
  player_dist = std::sqrt(pow(player.x - x, 2) + pow(player.y - y, 2));

  if (player_dist < proximityAttackThreshold) {
    direction = sprite_dir;
    state = attack_state;
  } else {
    state = randomWalk_state;
  }
}

void Monster::updatePosition(const Map &map, const Player &player, const double elapsed){
  //update position of the monster based on his set direction
  removePeriods(direction);
  float nx_sprite = x - cos(direction)*elapsed*speed;
  float ny_sprite = y - sin(direction)*elapsed*speed;

  if (int(nx_sprite)>=0 && int(nx_sprite)<int(map.w) && int(ny_sprite)>=0
      && int(ny_sprite)<int(map.h) && player_dist > proximityToPlayer) {
      if (map.is_empty(nx_sprite, ny_sprite)) { //new position is free of walls
        x = nx_sprite; y = ny_sprite;
      } else {//hitting a wall
        direction += M_PI + getRandom(-M_PI/6, M_PI/6); //turn around + small random angle
      }
  }
  player_dist = std::sqrt(pow(player.x - x, 2) + pow(player.y - y, 2)); //updating with new dist to sort
}

void Monster::action(const Map &map, Player &player, const double elapsed){
  switch(state) {
    case attack_state:
      attack(map, player, elapsed);
      break;
    case randomWalk_state:
      updatePosition(map, player,elapsed);
      break;
  }
}

void Monster::attack(const Map &map, Player &player, const double elapsed){
  updatePosition(map, player, elapsed);
  // says msg first time it enters here
  if (notfirstAttack) {sayMsg = true; notfirstAttack = false; firstMsg_secs = std::chrono::high_resolution_clock::now();}

  speed = 0.8; //increase speed if in proximity
  //enable monster to attack player
  timefromLastAttack_secs =  std::chrono::high_resolution_clock::now() - timeatLastAttack_secs; //time from last attack
  if (player_dist < distanceMonsterAttack && timefromLastAttack_secs.count() > timeAttackMonster && player.life > 0) {
    player.life -= dmgMonsterAttack;
    timeatLastAttack_secs = std::chrono::high_resolution_clock::now(); //reseting time at attack
  }
}

void Monster::manageDead(std::shared_ptr<Sprite> &sprite){
  sprite = nullptr; //just setting the sprite pointer to null to avoid drawing it in the next update
}

uint8_t Monster::changeColorLife(const uint8_t r){
  return (r-255)*life/100.0 + 255; //linear function
}

void Monster::removePeriods(float &angle_in){
  while (angle_in >  2*M_PI) angle_in -= 2*M_PI; // remove unncesessary periods from the relative direction
  while (angle_in <  -2*M_PI) angle_in += 2*M_PI;
}
