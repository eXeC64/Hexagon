#pragma once

class Polygon;

const double    GetTime();
const double    LInterp(const double pos, const double min, const double max);
const double    Clamp(const double val, const double min, const double max);
const double    Cycle(const double val, const double min, const double max);
const double    Max(const double a, const double b);
const int       Twist(const int direction, const int side, const int numSides);
const double    Normalise(double value, double min, double max);

void ConstructSideShape(Polygon &shape,
                                const int side, const int numSides,
                                const double in, const double out);
