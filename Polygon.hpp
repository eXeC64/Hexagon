#pragma once

#include <SDL/SDL.h>

class View
{
public:
    View();
    ~View();

    void SetViewRect(const double x1, const double y1, const double x2, const double y2);
    void SetRotation(const double rot);
    void SetViewport(const double w, const double h);
    void TransformPoint(double &x, double &y) const;
private:
    double m_left;
    double m_right;
    double m_top;
    double m_bottom;
    double m_rotation;
    double m_width;
    double m_height;
    double m_ct;
    double m_st;
};


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
