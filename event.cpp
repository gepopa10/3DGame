#include "event.h"

Event::Event(GameState& gs_in) : m_gs(gs_in),
                                 b_m_quit(false),
                                 m_shootAgain(false) {

}

Event::~Event(){

}

bool Event::processEvent(std::chrono::time_point<std::chrono::high_resolution_clock>& t1,
                         const std::chrono::time_point<std::chrono::high_resolution_clock>& t2){

  std::chrono::duration<double,  std::ratio<1>> elapsed_secs = t2 - t1;
  //m_shootAgain = false;
  if (elapsed_secs.count() > m_gs.player.reloadTime) {t1 = t2; m_shootAgain = true;}

  m_gs.player.fire = 0;

  if (SDL_PollEvent(&m_event)) {
      if (SDL_QUIT==m_event.type || (SDL_KEYDOWN==m_event.type && SDLK_ESCAPE==m_event.key.keysym.sym)) b_m_quit = true;
      if (SDL_KEYUP==m_event.type) { //SDL_KEYUP when user releases button
          if ('a'==m_event.key.keysym.sym || 'd'==m_event.key.keysym.sym) m_gs.player.turn = 0;
          if ('w'==m_event.key.keysym.sym || 's'==m_event.key.keysym.sym) m_gs.player.walk = 0;
          if (' '==m_event.key.keysym.sym)  m_gs.player.fire = 0;
          if ('m'==m_event.key.keysym.sym) m_gs.mapModeActive = false;
      }
      if (SDL_KEYDOWN==m_event.type) { //SDL_KEYDOWN when user presses
          if ('a'==m_event.key.keysym.sym) m_gs.player.turn = -1;
          if ('d'==m_event.key.keysym.sym) m_gs.player.turn =  1;
          if ('w'==m_event.key.keysym.sym) m_gs.player.walk =  1;
          if ('s'==m_event.key.keysym.sym) m_gs.player.walk = -1;
          if (' '==m_event.key.keysym.sym)
          {
            if (m_shootAgain) {m_gs.player.fire = 1; m_shootAgain = false;} //shoot only if we have reloaded
          }
          //setting gun
          if ('1'==m_event.key.keysym.sym) m_gs.player.weaponName = "knife";
          if ('2'==m_event.key.keysym.sym) m_gs.player.weaponName = "gun";
          if ('3'==m_event.key.keysym.sym) m_gs.player.weaponName = "rifle";
          if ('4'==m_event.key.keysym.sym) m_gs.player.weaponName = "machinegun";
          if ('m'==m_event.key.keysym.sym) m_gs.mapModeActive = true;
      }
  }

  return b_m_quit;
}
