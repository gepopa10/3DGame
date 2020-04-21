// window.h
#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <vector>
#include <memory>

#include "SDL.h"
#include <SDL_ttf.h>

#include "framebuffer.h"
#include "fonts.h"
#include "tinyraycaster.h"

class Window
{
  public:
    Window(FrameBuffer& fb_in, GameState& gs_in);
    ~Window();
    void display();
  protected:
    FrameBuffer&      m_fb;
    GameState&        m_gs;
    SDL_Window*       m_window;
    SDL_Renderer*     m_renderer;
    SDL_Texture*      m_framebuffer_texture;
    std::vector<std::shared_ptr<Font>> m_fonts;
};

#endif /* WINDOW_H */
