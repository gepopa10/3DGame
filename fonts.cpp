#include "fonts.h"

Font::Font(const std::vector<int>& location_in,
           const int& text_in_int,
           const char*& dummy,
           const std::vector<int>& color_in,
           const int& size_in,
           const char* path_font) :
           m_location(location_in),
           m_text_int(text_in_int),
           m_text(dummy),
           m_color(color_in),
           m_size(size_in),
           m_font(nullptr),
           m_colorSDL({ 0, 0, 0, 255 }),
           m_surface(nullptr),
           m_texture(nullptr){
  if(TTF_Init()==-1) {
    printf("TTF_Init: %s\n", TTF_GetError());
    exit(2);
  }

  m_font = TTF_OpenFont(path_font, m_size);
  m_colorSDL = { m_color[0], m_color[1], m_color[2], m_color[3] };

}

Font::Font(const std::vector<int>& location_in,
           const char*& text_in,
           const std::vector<int>& color_in,
           const int& size_in,
           const char* path_font) :
           m_location(location_in),
           m_text(text_in),
           m_text_int(size_in),
           m_color(color_in),
           m_size(size_in),
           m_font(nullptr),
           m_colorSDL({ 0, 0, 0, 255 }),
           m_surface(nullptr),
           m_texture(nullptr){
  if(TTF_Init()==-1) {
    printf("TTF_Init: %s\n", TTF_GetError());
    exit(2);
  }

  m_font = TTF_OpenFont(path_font, m_size);
  m_colorSDL = { m_color[0], m_color[1], m_color[2], m_color[3] };

}


Font::~Font(){
  TTF_CloseFont(m_font);
  TTF_Quit();
}

void Font::render(SDL_Renderer* renderer_in){

  if (!(m_text_int == m_size)){
    std::string s = std::to_string(m_text_int);
    char const *pchar = s.c_str();
    m_surface = TTF_RenderText_Solid(m_font, pchar, m_colorSDL); //we constructed the text with an int
  } else {
    m_surface = TTF_RenderText_Solid(m_font, m_text, m_colorSDL); //we constructed the text with an char*
  }

  m_texture = SDL_CreateTextureFromSurface(renderer_in, m_surface);
  int texW = 0;
  int texH = 0;
  int posx = m_location[0];
  int posy = m_location[1];
  SDL_QueryTexture(m_texture, NULL, NULL, &texW, &texH);
  SDL_Rect dstrect = { posx-texW/2, posy-texH/2, texW, texH }; //-texW/2 so location specify the middle of the text

  SDL_RenderCopy(renderer_in, m_texture, NULL, &dstrect);

  SDL_FreeSurface(m_surface);
  SDL_DestroyTexture(m_texture);
}
