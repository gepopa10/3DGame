#include "hitler.h"

Hitler::Hitler(float x_in,
               float y_in,
               Texture texture_in,
               size_t tex_id_in,
               float player_dist_in,
               float speed_in,
               float direction_in,
               bool aimed_in,
               int life_in):
               Boss(x_in,
                    y_in,
                    texture_in,
                    tex_id_in,
                    player_dist_in,
                    speed_in,
                    direction_in,
                    aimed_in,
                    life_in){

  std::chrono::duration<double,  std::ratio<1>> d_check(1);
  std::chrono::duration<double,  std::ratio<1>> d_move(0.5);
  std::chrono::duration<double,  std::ratio<1>> d_dead(0.2);

  checkAnimationTimes_secs = {d_check, d_check*2};
  moveAnimationTimes_secs = {d_move, d_move*2};
  shootAnimationTimes_secs = {d_move, d_move*2, d_move*3, d_move*4};
  dieAnimationTimes_secs = {d_dead, d_dead*2, d_dead*3, d_dead*4, d_dead*5, d_dead*6, d_dead*7};
}

void Hitler::updatePosition(const Map &map, const Player &player, const double elapsed){
  //update position of the monster based on his set direction
  removePeriods(direction);
  while (direction <  0) direction += 2*M_PI; //we make sure angle is not neg
  float nx_sprite = x + cos(direction)*elapsed*speed;
  float ny_sprite = y - sin(direction)*elapsed*speed;

  if (int(nx_sprite)>=0 && int(nx_sprite)<int(map.w) && int(ny_sprite)>=0
      && int(ny_sprite)<int(map.h) && player_dist > proximityToPlayer) {

      if (map.is_empty(nx_sprite + distanceWall*cos(direction), ny_sprite) &&
          map.is_empty(nx_sprite + distanceWall*cos(direction), ny_sprite - distanceWall*sin(direction)) &&
          map.is_empty(nx_sprite, ny_sprite - distanceWall*sin(direction)) &&
          map.is_empty(nx_sprite, ny_sprite) && //new position is free of walls from a distanceWall
          (distanceSameDirection <= maxDistanceSameDirection)){ //we dont travel too much in same direction except in attack mode!

        distanceSameDirection += std::sqrt(pow(x - nx_sprite, 2) + pow(y - ny_sprite, 2));
        x = nx_sprite; y = ny_sprite;

      } else {//hitting a wall or change direction because too much in same direction
        if (((fmod(direction,M_PI/2.0)-floor(fmod(direction,M_PI/2.0))) > 0.7)) {
          direction = directionIni;
        } //we reset the direction that was changed by attack_mode
        direction += M_PI; //opposite direction
        distanceSameDirection = 0;
      }
  }
  player_dist = std::sqrt(pow(player.x - x, 2) + pow(player.y - y, 2)); //updating with new dist to sort
}

void Hitler::action(const Map &map, Player &player, const double elapsed){
  if (firstMotion) {timeStartedAnime_secs = std::chrono::high_resolution_clock::now(); firstMotion = false;}
  animateMonster();
  switch(state) {
    case attack_state:
      distanceSameDirection = 0;
      attack(map, player, elapsed);
      break;
    case randomWalk_state:
      if (animationFinished) {animation = check; animationFinished = false;}
      updatePosition(map, player,elapsed);
      break;
    case dead_state:
      break;
  }
}

void Hitler::attack(const Map &map, Player &player, const double elapsed){

  speed = 0.3; //increase speed if in proximity
  //enable monster to attack player
  timefromLastAttack_secs =  std::chrono::high_resolution_clock::now() - timeatLastAttack_secs; //time from last attack
  if (player_dist < distanceMonsterAttack && timefromLastAttack_secs.count() > timeAttackMonster && player.life > 0) {
    player.life -= dmgMonsterAttack;
    animation = shoot;
    animationFinished = false;
    timeatLastAttack_secs = std::chrono::high_resolution_clock::now(); //reseting time at attack
  } else if (animationFinished){
    animation = move;
    // updatePosition(map, player, elapsed);
    if (player_dist <= proximityToPlayer) {animation = stay; timeStartedAnime_secs = std::chrono::high_resolution_clock::now();}
  }
  if (animation == move) updatePosition(map, player, elapsed);
}

void Hitler::animateMonster(){
  timeDurationCurrentAnime_secs =  std::chrono::high_resolution_clock::now() - timeStartedAnime_secs; //time from last attack

  switch(animation) {
    case check:
      tex_id = 25;
      if (timeDurationCurrentAnime_secs > checkAnimationTimes_secs[0] && timeDurationCurrentAnime_secs <= checkAnimationTimes_secs[1])
        tex_id = 28;
      else if (timeDurationCurrentAnime_secs > checkAnimationTimes_secs[1])
        {timeStartedAnime_secs = std::chrono::high_resolution_clock::now(); animationFinished = true;}
      break;
    case move:
      tex_id = 24;
      if (timeDurationCurrentAnime_secs > moveAnimationTimes_secs[0] && timeDurationCurrentAnime_secs < moveAnimationTimes_secs[1])
        tex_id = 26;
      else if (timeDurationCurrentAnime_secs > moveAnimationTimes_secs[1])
        {timeStartedAnime_secs = std::chrono::high_resolution_clock::now(); animationFinished = true;}
      break;
    case shoot:
      tex_id = 28;
      if (timeDurationCurrentAnime_secs > shootAnimationTimes_secs[0] && timeDurationCurrentAnime_secs < shootAnimationTimes_secs[1])
        tex_id = 29;
      else if (timeDurationCurrentAnime_secs > shootAnimationTimes_secs[1] && timeDurationCurrentAnime_secs < shootAnimationTimes_secs[2])
        tex_id = 30;
      else if (timeDurationCurrentAnime_secs > shootAnimationTimes_secs[2] && timeDurationCurrentAnime_secs < shootAnimationTimes_secs[3])
        tex_id = 31;
      else if (timeDurationCurrentAnime_secs > shootAnimationTimes_secs[3])
        {timeStartedAnime_secs = std::chrono::high_resolution_clock::now(); animationFinished = true;}
      break;
    case die:
      tex_id = 33;
      if (timeDurationCurrentAnime_secs > dieAnimationTimes_secs[0] && timeDurationCurrentAnime_secs < dieAnimationTimes_secs[1])
        tex_id = 34;
      else  if (timeDurationCurrentAnime_secs > dieAnimationTimes_secs[1] && timeDurationCurrentAnime_secs < dieAnimationTimes_secs[2])
        tex_id = 35;
      else  if (timeDurationCurrentAnime_secs > dieAnimationTimes_secs[2] && timeDurationCurrentAnime_secs < dieAnimationTimes_secs[3])
        tex_id = 36;
      else  if (timeDurationCurrentAnime_secs > dieAnimationTimes_secs[3] && timeDurationCurrentAnime_secs < dieAnimationTimes_secs[4])
        tex_id = 37;
      else  if (timeDurationCurrentAnime_secs > dieAnimationTimes_secs[4] && timeDurationCurrentAnime_secs < dieAnimationTimes_secs[5])
        tex_id = 38;
      else  if (timeDurationCurrentAnime_secs > dieAnimationTimes_secs[5] && timeDurationCurrentAnime_secs < dieAnimationTimes_secs[6])
        tex_id = 39;
      else if (timeDurationCurrentAnime_secs > dieAnimationTimes_secs[6])
        tex_id = 32; //dont set animationFinished to true because we want to keep this texture
      break;
    case stay:
      tex_id = 28;
  }
}

void Hitler::checkandUpdateState(const Map &map, const Player &player){
  float sprite_dir = atan2(y - player.y, x - player.x);
  while (sprite_dir - player.a >  M_PI) sprite_dir -= 2*M_PI; // remove unncesessary periods from the relative direction
  while (sprite_dir - player.a < -M_PI) sprite_dir += 2*M_PI;
  player_dist = std::sqrt(pow(player.x - x, 2) + pow(player.y - y, 2));

  if (player_dist < proximityAttackThreshold) {
    direction = M_PI - sprite_dir;
    state = attack_state;
    if (life<=0) state = dead_state; //we set it here because its the only place we can change states...
  } else {
    state = randomWalk_state;
    if (life<=0) state = dead_state;
  }
}

void Hitler::manageDead(std::shared_ptr<Sprite> &sprite){
  animation = die; animationFinished = false;
}
