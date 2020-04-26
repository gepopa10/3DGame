#ifndef GUARD_H
#define GUARD_H

#include "monsteranimated.h"
#include <math.h>
#include <map>

struct Guard : MonsterAnimated {

    // MonsterAnimated seems to inherite the States enum this from Monster
    // typedef enum {randomWalk_state, attack_state, dead_state} States; //state in which the animated monster is
    // States state = randomWalk_state;
    typedef enum {check, move, shoot, die, stay} Animations; //animation texture in which the animated monster is
    Animations animation = check;
    typedef enum {north, northeast, northwest, east, west, southeast, southwest, south} DirectionAnimation; //animation texture for direction while checking
    DirectionAnimation animationDirection;

    bool animationFinished = true;
    std::chrono::duration<double,  std::ratio<1>> timeDurationCurrentAnime_secs;
    std::chrono::time_point<std::chrono::high_resolution_clock> timeStartedAnime_secs = std::chrono::high_resolution_clock::now();
    std::vector<std::chrono::duration<double,  std::ratio<1>>> checkAnimationTimes_secs;
    std::vector<std::chrono::duration<double,  std::ratio<1>>> moveAnimationTimes_secs;
    std::vector<std::chrono::duration<double,  std::ratio<1>>> shootAnimationTimes_secs;
    std::vector<std::chrono::duration<double,  std::ratio<1>>> dieAnimationTimes_secs;

    const float proximityAttackThreshold = 3.0; //If the player is closer than proximityAttackThreshold from a monster it starts attacking
    const float proximityToPlayer = 1.0; //If the player is closer than proximityAttackThreshold from a monster, the monster doesnt go further (to avoid entering in the player)
    const float distanceMonsterAttack = 2.0; //if monster is closer to distanceMonsterAttack it can inflict damage to player
    const float timeAttackMonster = 1; //minimum time before monster can reattack
    const int dmgMonsterAttack = 5; //dmg a monster can do to player if he is closer than distanceMonsterAttack

    bool sayMsg = false;
    bool notfirstAttack = true;
    char* msgToSay = "guard";
    std::chrono::duration<double,  std::ratio<1>> msgTime_secs;
    std::chrono::duration<double,  std::ratio<1>> msgTimeLimit_secs;
    std::chrono::time_point<std::chrono::high_resolution_clock> firstMsg_secs;

    std::map<float, std::vector<int>> animationDirectionMap; //map that contains the animations vector depending on the direction (the key)
    std::map<float, std::vector<int>>::iterator animationDirectionMapIterator;
    std::vector<int> tex_ids_check;

    // using Monster::Monster; //inheriting constructor
    Guard(float x_in,
          float y_in,
          Texture texture_in,
          size_t tex_id_in = 0,
          float player_dist_in = 10000,
          float speed_in = 0.5,
          float direction_in = 0,
          bool aimed_in = 0,
          int life_in = 100);

    virtual void updatePosition(const Map &map, const Player &player, const double elapsed) override;
    virtual void action(const Map &map, Player &player, const double elapsed) override;
    virtual void attack(const Map &map, Player &player, const double elapsed) override;
    virtual void animateMonster();
    virtual void checkandUpdateState(const Map &map, const Player &player) override; //just to change proximityAttackThreshold
    virtual void manageDead(std::shared_ptr<Sprite> &sprite) override;


    // find the element in map that is closer to the key provided,
    // returns a iterator which can be access with iterator->second to get the value associated with the key
    std::map<float, std::vector<int>>::iterator nearestKey(std::map<float, std::vector<int>>& map, float key)
    {
        auto lower_bound = map.lower_bound(key);
        if (lower_bound == map.end()) return --lower_bound;
        auto upper_bound = lower_bound; upper_bound++;
        if (upper_bound == map.end()) return lower_bound;
        auto dist_to_lower = lower_bound->first - key;
        auto dist_to_upper = upper_bound->first - key;
        return (dist_to_upper < dist_to_lower) ? upper_bound : lower_bound;
    }

    virtual char* msg() override{

      msgTime_secs = std::chrono::high_resolution_clock::now() - firstMsg_secs;
      if (msgTime_secs > msgTimeLimit_secs) sayMsg = false;

      if (sayMsg)
        return msgToSay;
      else
        return "";
    };

};

#endif // GUARD_H
