#include "Polygon.hpp"

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

View::View()
{
    m_left = 0;
    m_right = 16;
    m_top = 9;
    m_bottom = 0;
    m_rotation = 0;
    m_width = 720;
    m_height = 480;
    m_ct = 0;
    m_st = 0;
}

View::~View()
{
}

void View::SetViewRect(const double x1, const double y1, const double x2, const double y2)
{
    m_left = x1;
    m_right = x2;
    m_top = y1;
    m_bottom = y2;
}

void View::SetRotation(const double rot)
{
    m_rotation = rot;
    m_ct = cos(m_rotation/(2*M_PI*10));
    m_st = sin(m_rotation/(2*M_PI*10));

}

void View::SetViewport(const double w, const double h)
{
    m_width = w;
    m_height = h;
}

void View::TransformPoint(double &x, double &y) const
{
    const double rx = x*m_ct - y*m_st;
    const double ry = x*m_st + y*m_ct;

    const double nx = Normalise(rx, m_right, m_left);
    const double ny = Normalise(ry, m_top, m_bottom);

    const double tx = nx * m_width;
    const double ty = ny * m_height;

    x = tx;
    y = ty;
}
