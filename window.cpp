#include "window.h"

Window::Window(FrameBuffer& fb_in, GameState& gs_in) : m_fb(fb_in),
                                                       m_gs(gs_in),
                                                       m_window(nullptr),
                                                       m_renderer(nullptr),
                                                       m_framebuffer_texture(nullptr){
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
      std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
      //return -1;
  }

  if (SDL_CreateWindowAndRenderer(m_fb.w, m_fb.h, SDL_WINDOWPOS_CENTERED | SDL_WINDOW_BORDERLESS, &m_window, &m_renderer)) { //deferenciate because it requires double pointers
      std::cerr << "Failed to create window and renderer: " << SDL_GetError() << std::endl;
      //return -1;
  }

  m_framebuffer_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, m_fb.w, m_fb.h);
  if (!m_framebuffer_texture) {
      std::cerr << "Failed to create framebuffer texture : " << SDL_GetError() << std::endl;
      //return -1;
  }

  //setting fonts

  //game name
  m_fonts.push_back(std::make_shared<Font>(std::vector<int> {m_fb.w/2, 30},                  //location
                                           m_gs.gameName,                                    //text (const *char or int reference)
                                           std::vector<int> {139, 0, 0, 255},                  //color
                                           80,                                               //size
                                           "../wolfenstein.ttf"));                           //font file

  //game messages from mobs
  m_fonts.push_back(std::make_shared<Font>(std::vector<int> {m_fb.w/2, m_fb.h/2+120},
                                          m_gs.gameplayMessage,
                                          std::vector<int> {255, 140, 0, 255},
                                          40,
                                          "../teen_bd.ttf"));
  //life jauge
  m_fonts.push_back(std::make_shared<Font>(std::vector<int> {m_fb.w -85, m_fb.h-25},
                                           m_gs.player.life, m_gs.gameplayMessage,
                                           std::vector<int> {255, 255, 255, 255},
                                           30,
                                           "../pixelz.ttf"));
}

Window::~Window(){
  SDL_DestroyTexture(m_framebuffer_texture);
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

void Window::display(){
  SDL_UpdateTexture(m_framebuffer_texture, NULL, reinterpret_cast<void *>(m_fb.img.data()), m_fb.w*4);
  SDL_RenderClear(m_renderer);

  //apply frambuffer
  SDL_RenderCopy(m_renderer, m_framebuffer_texture, NULL, NULL);
  //apply fonts on top
  for (auto& ite_m_fonts : m_fonts) {
    ite_m_fonts->render(m_renderer);
  }
  //render to window
  SDL_RenderPresent(m_renderer);
}
