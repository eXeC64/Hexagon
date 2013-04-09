#include "HexagonView.hpp"

#include <math.h>
#include <stdio.h>

sf::Color HSVtoRGB(const double h, const double s, const double v);
void ConstructSideShape(sf::ConvexShape &shape, const int side, const int numSides, const double in, const double out);
const double LInterp(const double pos, const double min, const double max);

HexagonView::HexagonView()
{
    m_model = 0;

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
    const double    hexagonRadius = 1.0;
    const double    playerRadius = 1.3;
    const int       numSides = m_model->GetNumSides();


    sf::View view(sf::FloatRect(-6.4,-3.6,12.8,7.2));
    view.setRotation(cos(time*0.1) * 500);

    rt->setView(view);

    //Draw bg
    sf::ConvexShape bgSide(4);
    for(int i = 0; i < numSides; ++i) {
        bgSide.setFillColor(HSVtoRGB(Hue(), Sat(), 1.0));
        const double in = hexagonRadius - 0.1 + 0.05 * sin(time);
        ConstructSideShape(bgSide, i, numSides, in, hexagonRadius);
        rt->draw(bgSide);

        if(i % 2) {
            bgSide.setFillColor(HSVtoRGB(Hue(), Sat(), 0.3));
        } else {
            bgSide.setFillColor(HSVtoRGB(Hue(), Sat(), 0.4));
        }

        ConstructSideShape(bgSide, i, numSides, 1.0, 32);
        rt->draw(bgSide);
    }

    //Draw obstacles
    sf::ConvexShape obsShape(4);
    obsShape.setFillColor(HSVtoRGB(Hue(), Sat(), 1.0));
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
    {
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

        const double pulseScale = 0.05 + 0.01 * sin(time * 10);

        playerShape.setPoint(0, sf::Vector2f( -0.10, -pulseScale ) );
        playerShape.setPoint(1, sf::Vector2f( 0, 0 ) );
        playerShape.setPoint(2, sf::Vector2f( -0.10, pulseScale ) );
        playerShape.setFillColor(HSVtoRGB(Hue(), Sat(), 1.0));
        playerShape.setPosition(posX,posY);
        playerShape.setRotation(pos * 360 / numSides);

        rt->draw(playerShape);
    }

    //Draw time
    char timeStr[256];
    sprintf(timeStr, "Time: %05.2f", time);

    sf::Text timeText(timeStr);

    rt->setView(rt->getDefaultView());
    timeText.setPosition(0,0);
    timeText.setCharacterSize(30);
    rt->draw(timeText);

}

const double HexagonView::Hue() const
{
    if(m_model) {
        return abs(m_baseHue + m_hueRange * sin(m_model->GetTime() * m_hueSpeed)) % 1;
    } else {
        return m_baseHue;
    }
}

const double HexagonView::Sat() const
{
    if(m_model && m_model->IsGameOver()) {
        return 0.5;
    } else {
        return 1.0;
    }
}

void HexagonView::SetBaseHue(const double hue)
{
    const double posHue = abs(hue);
    if(posHue <= 1) {
        m_baseHue = posHue;
    }
}

void HexagonView::SetHueRange(const double range)
{
    m_hueRange = abs(range);
}

void HexagonView::SetHueSpeed(const double speed)
{
    m_hueSpeed = speed;
}

sf::Color HSVtoRGB(const double h, const double s, const double v)
{
    double r, g, b;

    const int       i = floor(h * 6);
    const double    f = h *  6 - i;
    const double    p = v * (1 - s);
    const double    q = v * (1 - f * s);
    const double    t = v * (1 - (1 - f) * s);

    switch(i % 6){
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }

    return sf::Color(r * 255, g * 255, b * 255); 
}

void ConstructSideShape(sf::ConvexShape &shape, const int side, const int numSides, const double in, const double out)
{
    shape.setPointCount(4);
    shape.setPoint(0, sf::Vector2f(in * cos( (side*2*M_PI)/numSides ),
                                   in * sin( (side*2*M_PI)/numSides )));
    shape.setPoint(1, sf::Vector2f(out * cos( (side*2*M_PI)/numSides ),
                                   out * sin( (side*2*M_PI)/numSides )));
    shape.setPoint(2, sf::Vector2f(out * cos( ((side+1)*2*M_PI)/numSides ),
                                   out * sin( ((side+1)*2*M_PI)/numSides )));
    shape.setPoint(3, sf::Vector2f(in * cos( ((side+1)*2*M_PI)/numSides ),
                                   in * sin( ((side+1)*2*M_PI)/numSides )));
}

const double LInterp(const double pos, const double min, const double max)
{
    if(pos <= 0) {
        return min;
    } else if(pos >= 1) {
        return max;
    } else {
        return min + (max - min) * pos;
    }
}
