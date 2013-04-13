#pragma once

#include "../Uni/Lib110ct/Lib110ct.h"

#include "HexagonModel.hpp"

class Game
{
public:
    Game();
    ~Game();

    void                Run();

private:
    HexagonModel*       m_model;
    Win110ct*           m_window;
};
