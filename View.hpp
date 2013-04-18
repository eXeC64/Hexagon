#pragma once

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
