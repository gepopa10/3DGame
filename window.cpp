#include "window.h"
#include <SDL_ttf.h>

Window::Window(FrameBuffer& fb_in) : m_fb(fb_in),
                                     m_window(nullptr),
                                     m_renderer(nullptr),
                                     m_framebuffer_texture(nullptr){
  if (SDL_Init(SDL_INIT_VIDEO)) {
      std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
      //return -1;
  }

  if(TTF_Init()==-1) {
    printf("TTF_Init: %s\n", TTF_GetError());
    exit(2);
  }

  if (SDL_CreateWindowAndRenderer(m_fb.w, m_fb.h, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS, &m_window, &m_renderer)) { //deferenciate because it requires double pointers
      std::cerr << "Failed to create window and renderer: " << SDL_GetError() << std::endl;
      //return -1;
  }

  m_framebuffer_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, m_fb.w, m_fb.h);
  if (!m_framebuffer_texture) {
      std::cerr << "Failed to create framebuffer texture : " << SDL_GetError() << std::endl;
      //return -1;
  }
}

Window::~Window(){
  SDL_DestroyTexture(m_framebuffer_texture);
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  TTF_Quit();
  SDL_Quit();
}

void Window::display(){
  SDL_UpdateTexture(m_framebuffer_texture, NULL, reinterpret_cast<void *>(m_fb.img.data()), m_fb.w*4);
  SDL_RenderClear(m_renderer);
  SDL_RenderCopy(m_renderer, m_framebuffer_texture, NULL, NULL);
  //SDL_RenderPresent(m_renderer);

  TTF_Font * font = TTF_OpenFont("../pixelz.ttf", 25);
  SDL_Color color = { 255, 255, 255, 255 };
      SDL_Surface * surface = TTF_RenderText_Solid(font,
          "Welcome to Gigi Labs", color);
  SDL_Texture * texture = SDL_CreateTextureFromSurface(m_renderer, surface);

  int texW = 0;
  int texH = 0;
  int posx = m_fb.w/2;
  int posy = m_fb.h/2;
  SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
  SDL_Rect dstrect = { posx, posy, texW, texH };

  SDL_RenderCopy(m_renderer, texture, NULL, &dstrect);
  SDL_RenderPresent(m_renderer);

  SDL_DestroyTexture(texture);
  SDL_FreeSurface(surface);
  TTF_CloseFont(font);
}
