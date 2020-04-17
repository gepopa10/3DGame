#include "event.h"

Event::Event(Player& player_in) : m_player(player_in),
                                  b_m_quit(false) {

}

Event::~Event(){
}

bool Event::processEvent(){
  if (SDL_PollEvent(&m_event)) {
      if (SDL_QUIT==m_event.type || (SDL_KEYDOWN==m_event.type && SDLK_ESCAPE==m_event.key.keysym.sym)) b_m_quit = true;
      if (SDL_KEYUP==m_event.type) { //SDL_KEYUP when user releases button
          if ('a'==m_event.key.keysym.sym || 'd'==m_event.key.keysym.sym) m_player.turn = 0;
          if ('w'==m_event.key.keysym.sym || 's'==m_event.key.keysym.sym) m_player.walk = 0;
      }
      if (SDL_KEYDOWN==m_event.type) { //SDL_KEYDOWN when user presses
          if ('a'==m_event.key.keysym.sym) m_player.turn = -1;
          if ('d'==m_event.key.keysym.sym) m_player.turn =  1;
          if ('w'==m_event.key.keysym.sym) m_player.walk =  1;
          if ('s'==m_event.key.keysym.sym) m_player.walk = -1;
      }
  }

  return b_m_quit;
}
