#include "player.h"

void Player::drawVisor(FrameBuffer &fb) {
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

    //parameters to put weapon in the middle
    size_t screen_size = 400;
    int h_offset = 45;
    int v_offset = 130;

    for (size_t i=0; i<screen_size; i++) {
        if (h_offset+int(i)<0 || h_offset+i>=fb.w/2) continue;
        for (size_t j=0; j<screen_size; j++) {
            if (v_offset+int(j)<0 || v_offset+j>=fb.h) continue;
            uint32_t color = texture.get(i*texture.size_x/screen_size, j*texture.size_y/screen_size, weapon_text_id);
            uint8_t r,g,b,a;
            unpack_color(color, r, g, b, a);
            if (a>128){
              fb.set_pixel(fb.w/2 + h_offset+i, v_offset+j, color);
            }
          }
    }

    if (fire) {animationFinished = false; timeStartedAnime_secs = std::chrono::high_resolution_clock::now();}
    if (!animationFinished) animate();
}

void Player::animate(){
  timeDurationCurrentAnime_secs =  std::chrono::high_resolution_clock::now() - timeStartedAnime_secs; //duration from the start of the animation
  weapon_text_id = weapon_text_ids[0];
  if (timeDurationCurrentAnime_secs > fireAnimationTimes_secs[0] && timeDurationCurrentAnime_secs <= fireAnimationTimes_secs[1])
    weapon_text_id = weapon_text_ids[1];
  else if (timeDurationCurrentAnime_secs > fireAnimationTimes_secs[1] && timeDurationCurrentAnime_secs <= fireAnimationTimes_secs[2])
    weapon_text_id = weapon_text_ids[2];
  else if (timeDurationCurrentAnime_secs > fireAnimationTimes_secs[2] && timeDurationCurrentAnime_secs <= fireAnimationTimes_secs[3])
    weapon_text_id = weapon_text_ids[3];
  else if (timeDurationCurrentAnime_secs > fireAnimationTimes_secs[3] && timeDurationCurrentAnime_secs <= fireAnimationTimes_secs[4])
    weapon_text_id = weapon_text_ids[4];
  else if (timeDurationCurrentAnime_secs > fireAnimationTimes_secs[4])
    {timeStartedAnime_secs = std::chrono::high_resolution_clock::now(); animationFinished = true;}
}

void Player::render(FrameBuffer &fb){
  drawVisor(fb);
  drawLifeJauge(fb);
  drawWeapon(fb);
}
