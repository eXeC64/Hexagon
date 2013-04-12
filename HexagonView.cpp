#include "HexagonView.hpp"

#include <math.h>
#include <stdio.h>

#include "Util.hpp"


HexagonView::HexagonView()
{
    m_model = 0;

    m_drawPlayer = true;

    m_baseHue = 0;
    m_hueRange = 1.0;
    m_hueSpeed = 0.1;
}

HexagonView::~HexagonView()
{
}

void HexagonView::SetModel(HexagonModel* model)
{
    m_model = model;
}

void HexagonView::Draw(sf::RenderTarget* rt) const
{
    if(!m_model) {
        return;
    }

    const double    time = m_model->GetTime();
    const double    hexagonRadius = 1.0 + 0.15 * sin(time * 10);
    const double    playerRadius = 1.4;
    const int       numSides = m_model->GetNumSides();

    const double zoom = 1.0;
    const double w = zoom * 16;
    const double h = zoom * 9;
    sf::View view(sf::FloatRect(-w/2,-h/2,w,h));
    view.setRotation(m_model->GetRotation());

    rt->setView(view);

    //Draw bg
    sf::ConvexShape bgSide(4);
    for(int i = 0; i < numSides; ++i) {
        bgSide.setFillColor(HSVtoRGB(Hue(), Sat(), 0.8));
        const double in = hexagonRadius - 0.1 + 0.05 * sin(time * 5);
        ConstructSideShape(bgSide, i, numSides, in, hexagonRadius);
        rt->draw(bgSide);

        bgSide.setFillColor(HSVtoRGB(Hue(), Sat(), 0.2));
        ConstructSideShape(bgSide, i, numSides, 0, in);
        rt->draw(bgSide);

        if(i % 2) {
            bgSide.setFillColor(HSVtoRGB(Hue(), Sat(), 0.2));
        } else {
            bgSide.setFillColor(HSVtoRGB(Hue(), Sat(), 0.3));
        }

        ConstructSideShape(bgSide, i, numSides, hexagonRadius, 32);
        rt->draw(bgSide);
    }

    //Draw obstacles
    sf::ConvexShape obsShape(4);
    obsShape.setFillColor(HSVtoRGB(Hue(), Sat(), 0.8));
    for(int i = 0; i < numSides; i++) {
        Obstacle* obs = m_model->GetObstacle(i);

        while(obs) {
            const double pd = m_model->GetPlayerDistance();
            double start = obs->start - pd + playerRadius;
            const double end = start + obs->end - obs->start;

            if(start < hexagonRadius) {
                start = hexagonRadius;
            }

            if(end > hexagonRadius) {
                ConstructSideShape(obsShape, i, numSides, start, end);
                rt->draw(obsShape);
            }

            obs = obs->next;
        }
    }

    //Draw player
    if(m_drawPlayer) {
        sf::ConvexShape playerShape(3);
        const double pos = m_model->GetPlayerPosition();

        //Get the two corners the player is between
        const int posMin = floor(pos);
        const int posMax = ceil(pos);

        const double posMinX = playerRadius * cos(posMin * 2 * M_PI / numSides);
        const double posMinY = playerRadius * sin(posMin * 2 * M_PI / numSides);
        const double posMaxX = playerRadius * cos(posMax * 2 * M_PI / numSides);
        const double posMaxY = playerRadius * sin(posMax * 2 * M_PI / numSides);

        const double lerp = pos - posMin;

        const double posX = LInterp(lerp, posMinX, posMaxX);
        const double posY = LInterp(lerp, posMinY, posMaxY);

        const double pulseScale = 0.1 + 0.025 * sin(time * 10);
        const double turnMod = m_model->IsGameOver() ? 0 : m_model->GetPlayerDirection() * 15;

        playerShape.setPoint(0, sf::Vector2f( -0.20, -pulseScale ) );
        playerShape.setPoint(1, sf::Vector2f( 0, 0 ) );
        playerShape.setPoint(2, sf::Vector2f( -0.20, pulseScale ) );
        playerShape.setFillColor(HSVtoRGB(Hue(), Sat(), 1.0));
        playerShape.setPosition(posX,posY);
        playerShape.setRotation(pos * 360 / numSides + turnMod);

        rt->draw(playerShape);
    }
}

const double HexagonView::Hue() const
{
    if(m_model) {
        const double hueDiff = m_hueRange * sin(m_model->GetTime() * m_hueSpeed);
        const double hue = m_baseHue + hueDiff;
        return Cycle(hue, 0, 1);
    } else {
        return m_baseHue;
    }
}

const double HexagonView::Sat() const
{
    return 1.0;
}

void HexagonView::SetBaseHue(const double hue)
{
    m_baseHue = Clamp(hue, 0, 1);
}

void HexagonView::SetHueRange(const double range)
{
    m_hueRange = range;
}

void HexagonView::SetHueSpeed(const double speed)
{
    m_hueSpeed = speed;
}

void HexagonView::SetDrawPlayer(bool draw)
{
    m_drawPlayer = draw;
}
