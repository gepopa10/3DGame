#ifndef PLAYER_H
#define PLAYER_H

class Player {

  public:
    Player(float x_in,
                  float y_in,
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
                  a(a_in),
                  fov(fov_in),
                  turn(turn_in),
                  walk(walk_in),
                  fire(fire_in),
                  reloadTime(reloadTime_in),
                  speed(speed_in),
                  life(life_in),
                  distShoot(distShoot_in),
                  weapongDmgs(weapongDmgs_in){};
                  
    float x, y; // position
    float a;    // view direction
    float fov;  // field of view
    int turn, walk;
    bool fire;
    float reloadTime;
    float speed;
    int life;
    const float distShoot = 8.0; //in wall size 16 usually
    const size_t weapongDmgs = 25; //dmgs the weapon can make with 1 shoot to the monsters
};

#endif // PLAYER_H
