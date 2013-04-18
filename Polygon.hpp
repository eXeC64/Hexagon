#pragma once

#include <SDL/SDL.h>

class View;
class Polygon
{
public:
    Polygon(View* view, const int vertices = 6);
    ~Polygon();

    void            SetRGB(const char r, const char g, const char b);
    void            SetHSV(const double h, const double s, const double v);
    void            SetNumVertices(const int numVertices);
    void            SetVertex(const int v, const double x, const double y);
    void            Draw(SDL_Surface* rt) const;
private:
    int             m_numVertices;
    short*          m_verticesX;
    short*          m_verticesY;
    char            m_r;
    char            m_g;
    char            m_b;
    View*           m_view;
};
