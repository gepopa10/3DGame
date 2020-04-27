#include "sprite.h"

Sprite::Sprite(float x_in,
               float y_in,
               Texture texture_in,
               size_t tex_id_in,
               float player_dist_in):
               x(x_in),
               y(y_in),
               texture(texture_in),
               tex_id(tex_id_in),
               player_dist(player_dist_in){

}

void Sprite::draw_sprite(FrameBuffer &fb,
                         const std::vector<float> &depth_buffer,
                         const Player &player) {

    // absolute direction from the player to the sprite (in radians)
    float sprite_dir = atan2(y - player.y, x - player.x);
    while (sprite_dir - player.a >  M_PI) sprite_dir -= 2*M_PI; // remove unncesessary periods from the relative direction
    while (sprite_dir - player.a < -M_PI) sprite_dir += 2*M_PI;

    size_t sprite_screen_size = std::min(4000, static_cast<int>(fb.h/player_dist)); // screen sprite size
    int h_offset = (sprite_dir - player.a)*(fb.w)/(player.fov) + (fb.w/2) - sprite_screen_size/2; // do not forget the 3D view takes only a half of the framebuffer, thus fb.w/2 for the screen width
    int v_offset = fb.h/2 - sprite_screen_size/2;

    for (size_t i=0; i<sprite_screen_size; i++) {
        if (h_offset+int(i)<0 || h_offset+i>=fb.w) continue;
        if (depth_buffer[h_offset+i]<player_dist) continue; // this sprite column is occluded
        for (size_t j=0; j<sprite_screen_size; j++) {
            if (v_offset+int(j)<0 || v_offset+j>=fb.h) continue;
            uint32_t color = texture.get(i*texture.size_x/sprite_screen_size, j*texture.size_y/sprite_screen_size, tex_id);
            uint8_t r,g,b,a;
            unpack_color(color, r, g, b, a);
            if (a>128){
              fb.set_pixel( h_offset+i, v_offset+j, color);
            }
          }
    }
}
