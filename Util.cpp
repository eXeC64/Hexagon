#include "Util.hpp"

#include <math.h>
#include <stdlib.h>
#include <sys/time.h>


const void HSVtoRGB(const double h, const double s, const double v, char rgb[3])
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

    rgb[0] = r * 255;
    rgb[1] = g * 255;
    rgb[2] = b * 255;
}

const double GetTime()
{
    timeval time;

    gettimeofday(&time, 0);

    return (double)time.tv_sec + (double)time.tv_usec * 1000000;
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

