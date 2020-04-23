#ifndef MONSTERANIMATED_H
#define MONSTERANIMATED_H

#include "monster.h"

struct MonsterAnimated : Monster {

    // MonsterAnimated seems to inherite the States enum this from Monster
    // typedef enum {randomWalk_state, attack_state, dead_state} States; //state in which the animated monster is
    // States state = randomWalk_state;
    typedef enum {check, move, shoot, die} Animations; //animation texture in which the animated monster is
    Animations animation = check;
    bool animationFinished = true;
    std::chrono::duration<double,  std::ratio<1>> timeDurationCurrentAnime_secs;
    std::chrono::time_point<std::chrono::high_resolution_clock> timeStartedAnime_secs = std::chrono::high_resolution_clock::now();
    std::vector<std::chrono::duration<double,  std::ratio<1>>> checkAnimationTimes_secs;
    std::vector<std::chrono::duration<double,  std::ratio<1>>> moveAnimationTimes_secs;
    std::vector<std::chrono::duration<double,  std::ratio<1>>> shootAnimationTimes_secs;
    std::vector<std::chrono::duration<double,  std::ratio<1>>> dieAnimationTimes_secs;

    const float proximityAttackThreshold = 10.0; //If the player is closer than proximityAttackThreshold from a monster it starts attacking
    const float proximityToPlayer = 2.0; //If the player is closer than proximityAttackThreshold from a monster, the monster doesnt go further (to avoid entering in the player)
    const float distanceMonsterAttack = 4.0; //if monster is closer to distanceMonsterAttack it can inflict damage to player
    const float timeAttackMonster = 3; //minimum time before monster can reattack

    // using Monster::Monster; //inheriting constructor
    MonsterAnimated(float x_in,
                    float y_in,
                    Texture texture_in,
                    size_t tex_id_in = 0,
                    float speed_in = 0,
                    float player_dist_in = 10000,
                    float direction_in = 0,
                    bool aimed_in = 0,
                    int life_in = 0);
    virtual void updatePosition(const Map &map, const Player &player, const double elapsed) override;
    virtual void action(const Map &map, Player &player, const double elapsed) override;
    virtual void attack(const Map &map, Player &player, const double elapsed) override;
    virtual void animateMonster();

};

#endif // MONSTERANIMATED_H
