#ifndef HITLER_H
#define HITLER_H

#include "boss.h"

struct Hitler : Boss {

    typedef enum {check, move, shoot, die, stay} Animations; //animation texture in which the animated monster is
    Animations animation = check;
    bool animationFinished = true;
    std::chrono::duration<double,  std::ratio<1>> timeDurationCurrentAnime_secs;
    std::chrono::time_point<std::chrono::high_resolution_clock> timeStartedAnime_secs = std::chrono::high_resolution_clock::now();
    std::vector<std::chrono::duration<double,  std::ratio<1>>> checkAnimationTimes_secs;
    std::vector<std::chrono::duration<double,  std::ratio<1>>> moveAnimationTimes_secs;
    std::vector<std::chrono::duration<double,  std::ratio<1>>> shootAnimationTimes_secs;
    std::vector<std::chrono::duration<double,  std::ratio<1>>> dieAnimationTimes_secs;

    const float proximityAttackThreshold = 8.0; //If the player is closer than proximityAttackThreshold from a monster it starts attacking
    const float proximityToPlayer = 2.0; //If the player is closer than proximityAttackThreshold from a monster, the monster doesnt go further (to avoid entering in the player)
    const float distanceMonsterAttack = 4.0; //if monster is closer to distanceMonsterAttack it can inflict damage to player
    const float timeAttackMonster = 3; //minimum time before monster can reattack

    bool sayMsg = false;
    bool notfirstAttack = true;
    char* msgToSay = "Die, Allied Schweinehund Radu!"; //"Die, Allied Schweinehund Radu!"
    std::chrono::duration<double,  std::ratio<1>> msgTime_secs;
    std::chrono::duration<double,  std::ratio<1>> msgTimeLimit_secs;
    std::chrono::time_point<std::chrono::high_resolution_clock> firstMsg_secs;
    // using Monster::Monster; //inheriting constructor
    Hitler(float x_in,
           float y_in,
           Texture texture_in,
           size_t tex_id_in = 0,
           float speed_in = 0,
           float player_dist_in = 10000,
           float direction_in = 0,
           bool aimed_in = 0,
           int life_in = 100);

    virtual void updatePosition(const Map &map, const Player &player, const double elapsed) override;
    virtual void action(const Map &map, Player &player, const double elapsed) override;
    virtual void attack(const Map &map, Player &player, const double elapsed) override;
    virtual void animateMonster();
    virtual void checkandUpdateState(const Map &map, const Player &player) override; //just to change proximityAttackThreshold
    virtual void manageDead(std::shared_ptr<Sprite> &sprite) override;

    virtual char* msg() override{

      msgTime_secs = std::chrono::high_resolution_clock::now() - firstMsg_secs;
      if (msgTime_secs > msgTimeLimit_secs) sayMsg = false;

      if (sayMsg)
        return msgToSay;
      else
        return "";
    };

};

#endif // HITLER_H
