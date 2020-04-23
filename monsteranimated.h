#ifndef MONSTERANIMATED_H
#define MONSTERANIMATED_H

#include "monster.h"

struct MonsterAnimated : Monster {

    // MonsterAnimated seems to inherite the States enum this from Monster
    // typedef enum {randomWalk_state, attack_state, dead_state} States; //state in which the animated monster is
    // States state = randomWalk_state;
    typedef enum {check, move, shoot, die} Animations; //animation texture in which the animated monster is
    Animations animation = check;

    using Monster::Monster; //inheriting constructor


};

#endif // MONSTERANIMATED_H
