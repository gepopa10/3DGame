#include "textures.h"

Texture::Texture(const std::string filename, const uint32_t format, const int cols, const int rows) : img_w(0), img_h(0), count(0), size_x(0), img() {
    SDL_Surface *tmp = IMG_Load(filename.c_str());
    if (!tmp) {
        std::cerr << "Error in IMG_Load: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Surface *surface = SDL_ConvertSurfaceFormat(tmp, format, 0);
    SDL_FreeSurface(tmp);
    if (!surface) {
        std::cerr << "Error in SDL_ConvertSurfaceFormat: " << SDL_GetError() << std::endl;
        return;
    }

    int w = surface->w;
    int h = surface->h;
    std::cout << "w: " << w << "h: " << h << "surface->pitch" << surface->pitch << std::endl;

    count = cols*rows;
    size_x = w/cols;
    size_y = h/rows;
    img_w = w;
    img_h = h;
    std::cout << "img_h: " << img_h << "size_y: " << size_y << "(idx % img_h/size_y)" << (1 % int(img_h/size_y)) << std::endl;
    uint8_t *pixmap = reinterpret_cast<uint8_t *>(surface->pixels);

    img = std::vector<uint32_t>(w*h);
    for (int j=0; j<h; j++) {
        for (int i=0; i<w; i++) {
            uint8_t r = pixmap[(i+j*w)*4+0];
            uint8_t g = pixmap[(i+j*w)*4+1];
            uint8_t b = pixmap[(i+j*w)*4+2];
            uint8_t a = pixmap[(i+j*w)*4+3];
            img[i+j*w] = pack_color(r, g, b, a);
        }
    }
    SDL_FreeSurface(surface);
}

uint32_t Texture::get(const size_t i, const size_t j, const size_t idx) const {
    assert(i<size_x && j<size_y && idx<count);
    //return the i,j pixel in sprite from an index in a nxm sprites image -0-1-2->
    //                                                                    -3-4-5->
    //Works for for non square sprite from nxm sprites image file
    //take the modulo to eliminate rows since we pack the row of sprites fully before going to the next row so we need
    //to jump from floor(idx/(int(img_w/size_x)))*size_y when we go to next row and
    //(idx % int(img_w/size_x))*size_x to go to next column if idx < cols we want the modulo to return idx here
    //and floor(idx/(int(img_w/size_x))) should return 0 if idx < rows because it takes the whole of the idx divided by the nb
    //of cols so we stay in the first row.
    return img[i+(idx % int(img_w/size_x))*size_x + (j+ floor(idx/(int(img_w/size_x)))*size_y)*img_w];
}

std::vector<uint32_t> Texture::get_scaled_column(const size_t texture_id, const size_t tex_coord, const size_t column_height) const {
    assert(tex_coord<size_x && texture_id<count);
    std::vector<uint32_t> column(column_height);
    for (size_t y=0; y<column_height; y++) {
        column[y] = get(tex_coord, (y*size_x)/column_height, texture_id);
    }
    return column;
}
