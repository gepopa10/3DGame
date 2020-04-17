// event.h
#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include "SDL.h"
#include "player.h"

class Event
{
  public:
    Event(Player& player_in);
    ~Event();
    bool processEvent();
  protected:
    Player& m_player;
    SDL_Event m_event;
    bool b_m_quit;
};

#endif /* EVENT_H */
