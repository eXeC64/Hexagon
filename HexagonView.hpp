#pragma once

#include <SDL/SDL.h>

#include "HexagonModel.hpp"

class HexagonView
{
public:
    HexagonView();
    ~HexagonView();

    void            SetModel(HexagonModel* model);
    void            SetBaseHue(const double hue);
    void            SetHueRange(const double range);
    void            SetHueSpeed(const double speed);
    void            Draw(SDL_Surface* rt)               const;
    void            SetDrawPlayer(bool draw);
private:
    const double    Hue()   const;
    const double    Sat()   const;

    HexagonModel*   m_model;
    double          m_baseHue;
    double          m_hueRange;
    double          m_hueSpeed;

    bool            m_drawPlayer;
};
