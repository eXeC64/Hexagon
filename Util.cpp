#include "Util.hpp"
#include "Polygon.hpp"

#include <math.h>
#include <stdlib.h>
#include <time.h>

const double GetTime()
{
    timespec time;

    clock_gettime(CLOCK_MONOTONIC_RAW, &time);

    return (double)time.tv_sec + (double)time.tv_nsec * 0.000000001;
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

const double Normalise(double value, double min, double max)
{
    const double v = ((value - min) / (max - min));
    return v ? v : -v;
}

void ConstructSideShape(Polygon &shape,
                                const int side, const int numSides,
                                const double in, const double out)
{
    shape.SetNumVertices(4);
    shape.SetVertex(0, in * cos( (side*2*M_PI)/numSides),
                       in * sin( (side*2*M_PI)/numSides ));
    shape.SetVertex(1, out * cos( (side*2*M_PI)/numSides ),
                       out * sin( (side*2*M_PI)/numSides ));
    shape.SetVertex(2, out * cos( ((side+1)*2*M_PI)/numSides ),
                       out * sin( ((side+1)*2*M_PI)/numSides ));
    shape.SetVertex(3, in * cos( ((side+1)*2*M_PI)/numSides ),
                       in * sin( ((side+1)*2*M_PI)/numSides ));
}
