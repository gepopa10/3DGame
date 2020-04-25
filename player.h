#ifndef PLAYER_H
#define PLAYER_H

#include <chrono>
#include <string>
#include <map>

#include "textures.h"
#include "framebuffer.h"

class Player {

  public:

    Player(float x_in,
           float y_in,
           Texture texture_in,
           float a_in= M_PI/2.,
           float fov_in=M_PI/3.,
           int turn_in= 0,
           int walk_in= 0,
           bool fire_in= 0,
           float reloadTime_in= 0,
           float speed_in= 0.5,
           int life_in= 100);


    float x, y; // position
    Texture texture; //texture to put gun for instance
    float a;    // view direction
    float fov;  // field of view
    int turn, walk;
    bool fire;
    float reloadTime;
    float speed;
    int life;

    // weapons parameters
    struct Weapon {
      float distShoot; //in wall size 16 usually
      size_t weapongDmgs; //dmgs the weapon can make with 1 shoot to the monsters
      std::vector<int> weapon_text_ids;
      int weapon_text_id;

      Weapon(const float distShoot_in,const size_t weapongDmgs_in,const std::vector<int> weapon_text_ids_in):
             distShoot(distShoot_in),weapongDmgs(weapongDmgs_in),weapon_text_ids(weapon_text_ids_in){
               weapon_text_id = weapon_text_ids[0];
      }
    };
    Weapon currentWeapon; //needs to be initalized
    std::map<std::string, Weapon> animationWeaponsMap; //map that contains the animations vector depending on the weapon (the key)
    std::map<std::string, Weapon>::iterator animationWeaponsMapIterator;
    std::string weaponName = "gun";

    //weapon animation parameters
    bool animationFinished = true;
    std::chrono::duration<double,  std::ratio<1>> timeDurationCurrentAnime_secs;
    std::chrono::time_point<std::chrono::high_resolution_clock> timeStartedAnime_secs = std::chrono::high_resolution_clock::now();
    std::vector<std::chrono::duration<double,  std::ratio<1>>> fireAnimationTimes_secs;

    virtual void drawVisor(FrameBuffer &fb);
    virtual void drawLifeJauge(FrameBuffer &fb);
    virtual void drawWeapon(FrameBuffer &fb);
    virtual void animate();
    virtual void render(FrameBuffer &fb);
};

#endif // PLAYER_H
