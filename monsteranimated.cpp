#include "monsteranimated.h"

MonsterAnimated::MonsterAnimated(float x_in,
                                 float y_in,
                                 Texture texture_in,
                                 size_t tex_id_in,
                                 float player_dist_in,
                                 float speed_in,
                                 float direction_in,
                                 bool aimed_in,
                                 int life_in):
                                 Monster(x_in,
                                         y_in,
                                         texture_in,
                                         tex_id_in,
                                         player_dist_in,
                                         speed_in,
                                         direction_in,
                                         aimed_in,
                                         life_in),
                                 directionIni(direction_in){

}
