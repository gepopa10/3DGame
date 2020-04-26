#include "player.h"

Player::Player(float x_in,
               float y_in,
               Texture texture_in,
               float a_in,
               float fov_in,
               int turn_in,
               int walk_in,
               bool fire_in,
               float reloadTime_in,
               float speed_in,
               int life_in):
               x(x_in),
               y(y_in),
               texture(texture_in),
               a(a_in),
               fov(fov_in),
               turn(turn_in),
               walk(walk_in),
               fire(fire_in),
               reloadTime(reloadTime_in),
               speed(speed_in),
               life(life_in),
               currentWeapon(1,10,{0, 1, 2, 3, 4}){

  std::chrono::duration<double,  std::ratio<1>> d_fire(0.1);
  fireAnimationTimes_secs = {d_fire, d_fire*2, d_fire*3, d_fire*4, d_fire*5};

  //setting the animations in function of the weapon selected;
  animationWeaponsMap = {
    {"knife", Weapon(1,10,{0, 1, 2, 3, 4})},
    {"gun", Weapon(8,25,{5, 6, 7, 8, 9})},
    {"rifle", Weapon(6,50,{10, 11, 12, 13, 14})},
    {"machinegun", Weapon(6,100,{15, 16, 17, 18, 19})}
    };
}

void Player::drawVisor(FrameBuffer &fb) {
  // params of the players visor
  size_t visorWidth = 16; //in pixels
  size_t visorHeight = 16;
  size_t visorThickness = 2;
  size_t centerEmpty = 4;
  uint32_t visorColor = pack_color(255, 0, 0); //color of the visor

  fb.draw_rectangle(fb.w/2 - visorWidth/2, fb.h/2 - visorThickness/2, visorWidth/2 - centerEmpty/2, visorThickness, visorColor);
  fb.draw_rectangle(fb.w/2 + centerEmpty/2, fb.h/2 - visorThickness/2, visorWidth/2 - centerEmpty/2, visorThickness, visorColor);
  fb.draw_rectangle(fb.w/2 - visorThickness/2, fb.h/2 - visorHeight/2, visorThickness, visorHeight/2 -centerEmpty/2, visorColor);
  fb.draw_rectangle(fb.w/2 - visorThickness/2, fb.h/2 + centerEmpty/2, visorThickness, visorHeight/2 -centerEmpty/2, visorColor);
}

void Player::drawLifeJauge(FrameBuffer &fb) {
  // params of the players visor
  size_t lengthfull = 100; //in pixels
  size_t height = 30;
  size_t x = fb.w - 10 - lengthfull;
  size_t y = fb.h - 10 - height;

  uint32_t emptyColor = pack_color(0, 0, 0); //color of the empty life gauge
  uint32_t fullColor = pack_color(255, 0, 0); //color of the full life gauge

  fb.draw_rectangle(x, y, lengthfull*life/100.0, height, fullColor);
  fb.draw_rectangle(x + lengthfull*life/100.0, y, lengthfull - lengthfull*life/100.0, height, emptyColor);
}

void Player::drawWeapon(FrameBuffer &fb) {
    //setting the weapon
    animationWeaponsMapIterator = animationWeaponsMap.find(weaponName);
    if (animationWeaponsMapIterator != animationWeaponsMap.end())
      currentWeapon = animationWeaponsMapIterator->second;

    //doing the animation, allowed to start only when we fire
    if (fire) {animationFinished = false; timeStartedAnime_secs = std::chrono::high_resolution_clock::now();}
    if (!animationFinished) animate();

    //parameters to put weapon in the middle
    size_t screen_size = 400;
    int h_offset = 180;
    int v_offset = 220;

    for (size_t i=0; i<screen_size; i++) {
        if (h_offset+int(i)<0 || h_offset+i>=fb.w) continue;
        for (size_t j=0; j<screen_size; j++) {
            if (v_offset+int(j)<0 || v_offset+j>=fb.h) continue;
            uint32_t color = texture.get(i*texture.size_x/screen_size, j*texture.size_y/screen_size, currentWeapon.weapon_text_id);
            uint8_t r,g,b,a;
            unpack_color(color, r, g, b, a);
            if (a>128){
              fb.set_pixel(h_offset+i, v_offset+j, color);
            }
          }
    }
}

void Player::animate(){
  timeDurationCurrentAnime_secs =  std::chrono::high_resolution_clock::now() - timeStartedAnime_secs; //duration from the start of the animation
  currentWeapon.weapon_text_id = currentWeapon.weapon_text_ids[0];
  if (timeDurationCurrentAnime_secs > fireAnimationTimes_secs[0] && timeDurationCurrentAnime_secs <= fireAnimationTimes_secs[1])
    currentWeapon.weapon_text_id = currentWeapon.weapon_text_ids[1];
  else if (timeDurationCurrentAnime_secs > fireAnimationTimes_secs[1] && timeDurationCurrentAnime_secs <= fireAnimationTimes_secs[2])
    currentWeapon.weapon_text_id = currentWeapon.weapon_text_ids[2];
  else if (timeDurationCurrentAnime_secs > fireAnimationTimes_secs[2] && timeDurationCurrentAnime_secs <= fireAnimationTimes_secs[3])
    currentWeapon.weapon_text_id = currentWeapon.weapon_text_ids[3];
  else if (timeDurationCurrentAnime_secs > fireAnimationTimes_secs[3] && timeDurationCurrentAnime_secs <= fireAnimationTimes_secs[4])
    currentWeapon.weapon_text_id = currentWeapon.weapon_text_ids[4];
  else if (timeDurationCurrentAnime_secs > fireAnimationTimes_secs[4])
    {timeStartedAnime_secs = std::chrono::high_resolution_clock::now(); animationFinished = true;}
}

void Player::render(FrameBuffer &fb){
  drawVisor(fb);
  drawLifeJauge(fb);
  drawWeapon(fb);
}
