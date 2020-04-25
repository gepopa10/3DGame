#ifndef PLAYER_H
#define PLAYER_H

#include <chrono>

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
           int life_in= 100,
           const float distShoot_in= 8.0,
           const size_t weapongDmgs_in= 25):
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
           distShoot(distShoot_in),
           weapongDmgs(weapongDmgs_in){

             std::chrono::duration<double,  std::ratio<1>> d_fire(0.1);
             fireAnimationTimes_secs = {d_fire, d_fire*2, d_fire*3, d_fire*4, d_fire*5};
             weapon_text_ids = {5, 6, 7, 8, 9};
             weapon_text_id = weapon_text_ids[0];
           };

    float x, y; // position
    Texture texture; //texture to put gun for instance
    float a;    // view direction
    float fov;  // field of view
    int turn, walk;
    bool fire;
    float reloadTime;
    float speed;
    int life;
    const float distShoot = 8.0; //in wall size 16 usually
    const size_t weapongDmgs = 25; //dmgs the weapon can make with 1 shoot to the monsters
    int weapon_text_id;
    std::vector<int> weapon_text_ids;

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
