#include "Util.hpp"

#include <SFML/Graphics.hpp>
#include <math.h>


const sf::Color HSVtoRGB(const double h, const double s, const double v)
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

void ConstructSideShape(sf::ConvexShape &shape,
                                const int side, const int numSides,
                                const double in, const double out)
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

const double Clamp(const double val, const double min, const double max)
{
    if(val < min) {
        return min;
    } else if(val > max) {
        return max;
    } else {
        return val;
    }
}

const double Cycle(const double val, const double min, const double max)
{
    double i = val;
    const double range = max - min;
    while(i < min) {
        i += range;
    }
    while(i >= max) {
        i -= range;
    }
    return i;
}

const double Max(const double a, const double b)
{
    return a > b ? a : b;
}

const int Twist(const int direction, const int side, const int numSides)
{
    const int mult = direction ? 1 : -1;
    return abs(6 + mult * side) % numSides;
}

