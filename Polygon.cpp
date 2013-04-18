#include "Polygon.hpp"

#include "View.hpp"
#include "Util.hpp"

#include <SDL/SDL_gfxPrimitives.h>

Polygon::Polygon(View* view, const int vertices)
{
    m_view = view;
    m_numVertices = 0;
    SetNumVertices(vertices);    
}

Polygon::~Polygon()
{
    if(m_numVertices > 0) {
        delete[] m_verticesX;
        delete[] m_verticesY;
    }
}

void Polygon::SetRGB(const char r, const char g, const char b)
{
    m_r = r;
    m_g = g;
    m_b = b;
}

void Polygon::SetHSV(const double h, const double s, const double v)
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

    m_r = r * 255;
    m_g = g * 255;
    m_b = b * 255;
}

void Polygon::SetNumVertices(const int numVertices)
{
    if(m_numVertices > 0) {
        delete[] m_verticesX;
        delete[] m_verticesY;
    }

    m_numVertices = numVertices;

    if(m_numVertices > 0) {
        m_verticesX = new short[m_numVertices];
        m_verticesY = new short[m_numVertices];
    }

}

void Polygon::SetVertex(const int v, const double x, const double y)
{
    double tx = x;
    double ty = y;

    if(m_view) {
        m_view->TransformPoint(tx, ty);
    }

    if(v >= 0 && v < m_numVertices) {
        m_verticesX[v] = (short)tx;
        m_verticesY[v] = (short)ty;
    }
}

void Polygon::Draw(SDL_Surface* rt) const
{
    if(m_numVertices > 3) {
        filledPolygonRGBA(rt, m_verticesX, m_verticesY, m_numVertices, m_r, m_g, m_b, 255);
    }
}

