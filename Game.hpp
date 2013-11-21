#pragma once

#include <SDL2/SDL.h>

#include "HexagonModel.hpp"

class Game
{
public:
    Game();
    ~Game();

    void                Run();

private:
    HexagonModel*       m_model;
    SDL_Window*         m_window;
    SDL_Surface*        m_surface;
};
