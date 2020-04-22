#ifndef MONSTERANIMATED_H
#define MONSTERANIMATED_H

#include "monster.h"

struct MonsterAnimated : Monster {

    typedef enum {Walk, Attack, Die} State; //state in which the animated sprite is
    State m_state = Walk;

    using Monster::Monster; //inheriting constructor
    void setState(State state_in);
};

#endif // MONSTERANIMATED_H
