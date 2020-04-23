#include "monsteranimated.h"

MonsterAnimated::MonsterAnimated(float x_in,
                                 float y_in,
                                 Texture texture_in,
                                 size_t tex_id_in,
                                 float player_dist_in,
                                 float speed_in,
                                 float direction_in,
                                 bool aimed_in,
                                 int life_in):
                                 Monster(x_in,
                                         y_in,
                                         texture_in,
                                         tex_id_in,
                                         player_dist_in,
                                         speed_in,
                                         direction_in,
                                         aimed_in,
                                         life_in){

  // animationFinished = true;
  //timeDurationCurrentAnime_secs(0);
  std::chrono::duration<double,  std::ratio<1>> d1(1);
  std::chrono::duration<double,  std::ratio<1>> d2(2);
  std::chrono::duration<double,  std::ratio<1>> d3(3);
  std::chrono::duration<double,  std::ratio<1>> d4(4);
  checkAnimationTimes_secs = {d1, d2};
  moveAnimationTimes_secs = {d1, d2};
  shootAnimationTimes_secs = {d1, d2, d3, d4};
  dieAnimationTimes_secs = {d1, d2, d3};
}

void MonsterAnimated::updatePosition(const Map &map, const Player &player, const double elapsed){
  //update position of the monster based on his set direction
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

void MonsterAnimated::action(const Map &map, Player &player, const double elapsed){
  animateMonster();
  switch(state) {
    case attack_state:
      attack(map, player, elapsed);
      break;
    case randomWalk_state:
      if (animationFinished) {animation = check; animationFinished = false;}
      updatePosition(map, player,elapsed);
      break;
  }
}

void MonsterAnimated::attack(const Map &map, Player &player, const double elapsed){

  speed = 0.3; //increase speed if in proximity
  //enable monster to attack player
  timefromLastAttack_secs =  std::chrono::high_resolution_clock::now() - timeatLastAttack_secs; //time from last attack
  if (player_dist < distanceMonsterAttack && timefromLastAttack_secs.count() > timeAttackMonster && player.life > 0) {
    player.life -= dmgMonsterAttack;
    animation = shoot;
    animationFinished = false;
    timeatLastAttack_secs = std::chrono::high_resolution_clock::now(); //reseting time at attack
  } else {
    //updatePosition(map, player, elapsed);
    if (animationFinished) {animation = move; updatePosition(map, player, elapsed);}
  }
}

void MonsterAnimated::animateMonster(){
  timeDurationCurrentAnime_secs =  std::chrono::high_resolution_clock::now() - timeStartedAnime_secs; //time from last attack

  switch(animation) {
    case check:
      tex_id = 0;
      if (timeDurationCurrentAnime_secs > checkAnimationTimes_secs[0] && timeDurationCurrentAnime_secs <= checkAnimationTimes_secs[1])
        tex_id = 2;
      else if (timeDurationCurrentAnime_secs > checkAnimationTimes_secs[1])
        {timeStartedAnime_secs = std::chrono::high_resolution_clock::now(); animationFinished = true;}
      break;
    case move:
      tex_id = 1;
      if (timeDurationCurrentAnime_secs > moveAnimationTimes_secs[0] && timeDurationCurrentAnime_secs < moveAnimationTimes_secs[1])
        tex_id = 3;
      else if (timeDurationCurrentAnime_secs > moveAnimationTimes_secs[1])
        {timeStartedAnime_secs = std::chrono::high_resolution_clock::now(); animationFinished = true;}
      break;
    case shoot:
      tex_id = 4;
      if (timeDurationCurrentAnime_secs > shootAnimationTimes_secs[0] && timeDurationCurrentAnime_secs < shootAnimationTimes_secs[1])
        tex_id = 5;
      else if (timeDurationCurrentAnime_secs > shootAnimationTimes_secs[1] && timeDurationCurrentAnime_secs < shootAnimationTimes_secs[2])
        tex_id = 6;
      else if (timeDurationCurrentAnime_secs > shootAnimationTimes_secs[2] && timeDurationCurrentAnime_secs < shootAnimationTimes_secs[3])
        tex_id = 7;
      else if (timeDurationCurrentAnime_secs > shootAnimationTimes_secs[3])
        {timeStartedAnime_secs = std::chrono::high_resolution_clock::now(); animationFinished = true;}
      break;
    case die:
      tex_id = 8;
      if (timeDurationCurrentAnime_secs > dieAnimationTimes_secs[0] && timeDurationCurrentAnime_secs > dieAnimationTimes_secs[1])
        tex_id = 9;
      else if (timeDurationCurrentAnime_secs > dieAnimationTimes_secs[1] && timeDurationCurrentAnime_secs > dieAnimationTimes_secs[2])
        tex_id = 10;
      else if (timeDurationCurrentAnime_secs > dieAnimationTimes_secs[2])
        {timeStartedAnime_secs = std::chrono::high_resolution_clock::now(); animationFinished = true;}
      break;
  }
}
