#include "guard.h"

Guard::Guard(float x_in,
             float y_in,
             Texture texture_in,
             size_t tex_id_in,
             float player_dist_in,
             float speed_in,
             float direction_in,
             bool aimed_in,
             int life_in):
             MonsterAnimated(x_in,
                             y_in,
                             texture_in,
                             tex_id_in,
                             player_dist_in,
                             speed_in,
                             direction_in,
                             aimed_in,
                             life_in),
             msgTime_secs(0),
             msgTimeLimit_secs(5){

  std::chrono::duration<double,  std::ratio<1>> d_check(0.3);
  std::chrono::duration<double,  std::ratio<1>> d_dead(0.4);

  checkAnimationTimes_secs = {d_check, d_check*2, d_check*3, d_check*4, d_check*5};
  moveAnimationTimes_secs = {d_check, d_check*2, d_check*3, d_check*4, d_check*5};
  shootAnimationTimes_secs = {d_check, d_check*2, d_check*3, d_check*4};
  dieAnimationTimes_secs = {d_dead, d_dead*2, d_dead*3, d_dead*4, d_dead*5};

  //setting the animations in function of the angleAnimation;
  tex_ids_check = {0, 8, 16, 24, 32};
  animationDirectionMap = {
    {0, {6, 14, 22, 30, 38}}, //east
    {M_PI/4.0, {5, 13, 21, 29, 37}}, //northeast
    {2*M_PI/4.0, {4, 12, 20, 28, 36}}, //north
    {3*M_PI/4.0, {3, 11, 19, 27, 35}}, //northwest
    {4*M_PI/4.0, {2, 10, 18, 26, 34}}, //west
    {5*M_PI/4.0, {1, 9, 17, 25, 33}}, //southwest
    {6*M_PI/4.0, {0, 8, 16, 24, 32}}, //south
    {7*M_PI/4.0, {7, 15, 23, 31, 39}}, //southeast
    };

  // std::cout<<"----------------- Map --------------------: "<<std::endl;
  // std::cout<< (animationDirectionMap.lower_bound(5.5) == animationDirectionMap.end()) <<std::endl;
  // std::cout<<nearest_key(animationDirectionMap, 5.5)->second[1]<<std::endl;
  // std::cout<<animationDirectionMap.lower_bound(0.1)->second[1]<<std::endl;
  // std::cout<<animationDirectionMap.lower_bound(0.1)->second[2]<<std::endl;
  // std::cout<<"------------------------------------------- "<<std::endl;

}

void Guard::updatePosition(const Map &map, const Player &player, const double elapsed){
  //update position of the monster based on his set direction
  removePeriods(direction); // direction is //anti-clockwise
  while (direction <  0) direction += 2*M_PI; //we make sure angle is not neg
  float nx_sprite = x + cos(direction)*elapsed*speed; //negative to go in opposite direction
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
        if (((fmod(direction,M_PI/4.0)-floor(fmod(direction,M_PI/4.0))) > 0.7)) {
          direction = directionIni;
        } //we reset the direction that was changed by attack_mode

        direction += M_PI/4.0; //opposite direction
        distanceSameDirection = 0;
      }

  }

  // animationDirection setting
  float angleAnimation = direction - (6*M_PI/4.0 - player.a);
  while (angleAnimation <  0) angleAnimation += 2*M_PI; //we make sure angle is not neg
  // we use the angleAnimation (the key) to find the first element that is >= to this key
  // and that corresponds to the animation to use
  animationDirectionMapIterator = nearestKey(animationDirectionMap, angleAnimation);
  tex_ids_check = animationDirectionMapIterator->second; //we set the corresponding animation text_ids

  player_dist = std::sqrt(pow(player.x - x, 2) + pow(player.y - y, 2)); //updating with new dist to sort
}

void Guard::action(const Map &map, Player &player, const double elapsed){
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

void Guard::attack(const Map &map, Player &player, const double elapsed){
  //setting sayMsg to true on the first enter of attack and starting timer
  if (notfirstAttack) {sayMsg = true; notfirstAttack = false; firstMsg_secs = std::chrono::high_resolution_clock::now();}

  speed = 0.8; //increase speed if in proximity
  //enable monster to attack player
  timefromLastAttack_secs =  std::chrono::high_resolution_clock::now() - timeatLastAttack_secs; //time from last attack
  if (player_dist < distanceMonsterAttack && timefromLastAttack_secs.count() > timeAttackMonster && player.life > 0) {
    player.life -= dmgMonsterAttack;
    animation = shoot;
    animationFinished = false;
    timeatLastAttack_secs = std::chrono::high_resolution_clock::now(); //reseting time at attack
  } else if (animationFinished){
    animation = move;
    if (player_dist <= proximityToPlayer) {animation = stay; timeStartedAnime_secs = std::chrono::high_resolution_clock::now();}
  }
  if (animation == move) updatePosition(map, player, elapsed);
}

void Guard::animateMonster(){
  timeDurationCurrentAnime_secs =  std::chrono::high_resolution_clock::now() - timeStartedAnime_secs; //time from last attack

  switch(animation) {

    case check:
      tex_id = tex_ids_check[0];
      if (timeDurationCurrentAnime_secs > checkAnimationTimes_secs[0] && timeDurationCurrentAnime_secs <= checkAnimationTimes_secs[1])
        tex_id = tex_ids_check[1];
      else if (timeDurationCurrentAnime_secs > checkAnimationTimes_secs[1] && timeDurationCurrentAnime_secs <= checkAnimationTimes_secs[2])
        tex_id = tex_ids_check[2];
      else if (timeDurationCurrentAnime_secs > checkAnimationTimes_secs[2] && timeDurationCurrentAnime_secs <= checkAnimationTimes_secs[3])
        tex_id = tex_ids_check[3];
      else if (timeDurationCurrentAnime_secs > checkAnimationTimes_secs[3] && timeDurationCurrentAnime_secs <= checkAnimationTimes_secs[4])
        tex_id = tex_ids_check[4];
      else if (timeDurationCurrentAnime_secs > checkAnimationTimes_secs[4])
        {timeStartedAnime_secs = std::chrono::high_resolution_clock::now(); animationFinished = true;}
      break;

    case move:
    tex_id = 0;
    if (timeDurationCurrentAnime_secs > moveAnimationTimes_secs[0] && timeDurationCurrentAnime_secs <= moveAnimationTimes_secs[1])
      tex_id = 8;
    else if (timeDurationCurrentAnime_secs > moveAnimationTimes_secs[1] && timeDurationCurrentAnime_secs <= moveAnimationTimes_secs[2])
      tex_id = 16;
    else if (timeDurationCurrentAnime_secs > moveAnimationTimes_secs[2] && timeDurationCurrentAnime_secs <= moveAnimationTimes_secs[3])
      tex_id = 24;
    else if (timeDurationCurrentAnime_secs > moveAnimationTimes_secs[3] && timeDurationCurrentAnime_secs <= moveAnimationTimes_secs[4])
        tex_id = 32;
    else if (timeDurationCurrentAnime_secs > moveAnimationTimes_secs[4])
      {timeStartedAnime_secs = std::chrono::high_resolution_clock::now(); animationFinished = true;}
    break;

    case shoot:
      tex_id = 47;
      if (timeDurationCurrentAnime_secs > shootAnimationTimes_secs[0] && timeDurationCurrentAnime_secs < shootAnimationTimes_secs[1])
        tex_id = 48;
      else if (timeDurationCurrentAnime_secs > shootAnimationTimes_secs[1] && timeDurationCurrentAnime_secs < shootAnimationTimes_secs[2])
        tex_id = 49;
      else if (timeDurationCurrentAnime_secs > shootAnimationTimes_secs[2] && timeDurationCurrentAnime_secs < shootAnimationTimes_secs[3])
        tex_id = 50;
      else if (timeDurationCurrentAnime_secs > shootAnimationTimes_secs[3])
        {timeStartedAnime_secs = std::chrono::high_resolution_clock::now(); animationFinished = true;}
      break;

    case die:
      tex_id = 40;
      if (timeDurationCurrentAnime_secs > dieAnimationTimes_secs[0] && timeDurationCurrentAnime_secs < dieAnimationTimes_secs[1])
        tex_id = 41;
      else if (timeDurationCurrentAnime_secs > dieAnimationTimes_secs[1] && timeDurationCurrentAnime_secs < dieAnimationTimes_secs[2])
        tex_id = 42;
      else if (timeDurationCurrentAnime_secs > dieAnimationTimes_secs[2] && timeDurationCurrentAnime_secs < dieAnimationTimes_secs[3])
        tex_id = 43;
      else if (timeDurationCurrentAnime_secs > dieAnimationTimes_secs[3])
        tex_id = 44; //dont set animationFinished to true because we want to keep this texture
      break;

    case stay:
      tex_id = 47;
  }
}

void Guard::checkandUpdateState(const Map &map, const Player &player){
  float sprite_dir = atan2(y - player.y, x - player.x); //clockwise
  while (sprite_dir - player.a >  M_PI) sprite_dir -= 2*M_PI; // remove unncesessary periods from the relative direction
  while (sprite_dir - player.a < -M_PI) sprite_dir += 2*M_PI;
  player_dist = std::sqrt(pow(player.x - x, 2) + pow(player.y - y, 2));

  if (player_dist < proximityAttackThreshold) {
    direction = M_PI - sprite_dir; //M_PI-sprite_dir to converte from clockwise to anticlockwise
    state = attack_state;
    if (life<=0) state = dead_state; //we set it here because its the only place we can change states...
  } else {
    state = randomWalk_state;
    if (life<=0) state = dead_state;
  }
}

void Guard::manageDead(std::shared_ptr<Sprite> &sprite){
  animation = die; animationFinished = false;
}
