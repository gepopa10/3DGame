#include "window.h"

Window::Window(FrameBuffer& fb_in) : m_fb(fb_in),
                                    m_window(nullptr),
                                    m_renderer(nullptr),
                                    m_framebuffer_texture(nullptr){
  if (SDL_Init(SDL_INIT_VIDEO)) {
      std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
      //return -1;
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
  SDL_Quit();
}

void Window::display(){
  SDL_UpdateTexture(m_framebuffer_texture, NULL, reinterpret_cast<void *>(m_fb.img.data()), m_fb.w*4);
  SDL_RenderClear(m_renderer);
  SDL_RenderCopy(m_renderer, m_framebuffer_texture, NULL, NULL);
  SDL_RenderPresent(m_renderer);
}
