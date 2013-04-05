#pragma once

#include <SFML/Graphics.hpp>

#include "HexagonModel.hpp"

class Game
{
public:
    Game();
    ~Game();

    void                Run();

private:
    HexagonModel*       m_model;
    sf::RenderWindow*   m_window;
};
