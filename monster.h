#ifndef MONSTER_H
#define MONSTER_H

#include "sprite.h"
#include "map.h"

struct Monster : Sprite {

    float speed;
    float direction;
    bool aimed;
    int life;
    std::chrono::duration<double,  std::ratio<1>> timefromLastAttack_secs;
    std::chrono::time_point<std::chrono::high_resolution_clock> timeatLastAttack_secs = std::chrono::high_resolution_clock::now();

    const float proximityAttackThreshold = 4.0; //If the player is closer than 4 mapcells from a monster it starts attacking
    const float proximityToPlayer = 0.5; //If the player is closer than 1 mapcells from a monster, the monster doesnt go further (to avoid entering in the player)
    const float distanceMonsterAttack = 1; //if monster is closer to distanceMonsterAttack it can inflict damage to player
    const int dmgMonsterAttack = 10; //dmg a monster can do to player if he is closer than distanceMonsterAttack
    const float timeAttackMonster = 1; //minimum time before monster can reattack
    typedef enum {randomWalk_state, attack_state, dead_state} States; //state in which the monster is
    States state = randomWalk_state;

    Monster(float x_in,
            float y_in,
            Texture texture_in,
            size_t tex_id_in = 0,
            float speed_in = 0,
            float player_dist_in = 10000,
            float direction_in = 0,
            bool aimed_in = 0,
            int life_in = 0);

    virtual void draw_sprite(FrameBuffer &fb,
                             const std::vector<float> &depth_buffer,
                             const Player &player) override;

    virtual void update(const Map &map, Player &player, const double elapsed);
    virtual void attacked(const Map &map, const Player &player);
    virtual void checkandUpdateState(const Map &map, const Player &player);
    virtual void updatePosition(const Map &map, const Player &player, const double elapsed);
    virtual void action(const Map &map, Player &player, const double elapsed);
    virtual void attack(const Map &map, Player &player, const double elapsed);
    virtual void manageDead(std::shared_ptr<Sprite> &sprite);
    virtual uint8_t changeColorLife(const uint8_t r);

};

#endif // MONSTER_H
