// window.h
#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <memory>
#include "SDL.h"

#include "framebuffer.h"

class Window
{
  public:
    Window(FrameBuffer& fb_in);
    ~Window();
    void display();
  protected:
    FrameBuffer&      m_fb;
    SDL_Window*       m_window;
    SDL_Renderer*     m_renderer;
    SDL_Texture*      m_framebuffer_texture;
};

#endif /* WINDOW_H */
