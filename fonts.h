// fonts.h
#ifndef FONTS_H
#define FONTS_H

#include <iostream>
#include <vector>
#include <string>

#include "SDL.h"
#include <SDL_ttf.h>

#include "framebuffer.h"

class Font
{
  public:
    //constructor with an int ref (to change dynamically)
    Font(const std::vector<int>& location_in,
         const int& text_in_int,
         const char*& dummy, //needed to initialize m_text
         const std::vector<int>& color_in = { 0, 0, 0, 255 },
         const int& size_in = 25);
    //constructor with a char pointer (text)
    Font(const std::vector<int>& location_in,
         const char*& text_in,
         const std::vector<int>& color_in = { 0, 0, 0, 255 },
         const int& size_in = 25);

    ~Font();
    void render(SDL_Renderer* renderer_in);
    const char*& m_text;
    const int& m_size;
    const int& m_text_int;
  protected:
    const std::vector<int> m_location;
    const std::vector<int> m_color;
    TTF_Font * m_font;
    SDL_Color m_colorSDL;
    SDL_Surface * m_surface;
    SDL_Texture * m_texture;
};

#endif /* FONTS_H */
