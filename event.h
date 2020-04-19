// event.h
#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <chrono>

#include "SDL.h"

#include "player.h"

class Event
{
  public:
    Event(Player& player_in);
    ~Event();
    bool processEvent(std::chrono::time_point<std::chrono::high_resolution_clock>& t1,
                      const std::chrono::time_point<std::chrono::high_resolution_clock>& t2);
  protected:
    Player& m_player;
    SDL_Event m_event;
    bool b_m_quit;
    bool m_shootAgain;
};

#endif /* EVENT_H */
