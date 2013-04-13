#pragma once

namespace sf
{
    class Color;
    class ConvexShape;
}

const double    GetTime();

const double    LInterp(const double pos, const double min, const double max);
const double    Clamp(const double val, const double min, const double max);
const double    Cycle(const double val, const double min, const double max);
const double    Max(const double a, const double b);
const int       Twist(const int direction, const int side, const int numSides);

const void      HSVtoRGB(const double h, const double s, const double v, char rgb[3]);
