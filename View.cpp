#include "View.hpp"

#include "Util.hpp"
#include <math.h>

View::View()
{
    m_left = 0;
    m_right = 16;
    m_top = 9;
    m_bottom = 0;
    m_rotation = 0;
    m_width = 1280;
    m_height = 720;
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
