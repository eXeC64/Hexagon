#include "HexagonView.hpp"

#include <math.h>
#include <stdio.h>

#include <cairo/cairo.h>

#include "Util.hpp"

HexagonView::HexagonView()
{
    m_model = 0;

    m_drawPlayer = true;

    m_baseHue = 0;
    m_hueRange = 1.0;
    m_hueSpeed = 0.1;
}

HexagonView::~HexagonView()
{
}

void HexagonView::SetModel(HexagonModel* model)
{
    m_model = model;
}

void HexagonView::Draw(SDL_Surface* s) const
{
    if(!m_model) {
        return;
    }

    const double    time = m_model->GetTime();
    const double    hexagonRadius = 1.0 + 0.15 * sin(time * 10);
    const double    playerRadius = 1.4;
    const int       numSides = m_model->GetNumSides();

    cairo_surface_t* surf = cairo_image_surface_create_for_data((unsigned char*)s->pixels, CAIRO_FORMAT_RGB24, s->w, s->h, s->pitch);
    cairo_t* cr = cairo_create(surf);

    double colour[3];

    HSVtoRGB(Hue(), Sat(), 0.2, colour);
    cairo_set_source_rgb(cr, colour[0], colour[1], colour[2]);
    cairo_paint(cr);

    cairo_translate(cr, (s->w)/2, (s->h)/2);
    cairo_scale(cr, s->w/16, s->h/9);
    cairo_rotate(cr, m_model->GetRotation());

    cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
    cairo_move_to(cr, 0.0, 0.0);
    cairo_line_to(cr, 1.0, 1.0);
    cairo_set_line_width(cr, 0.01);
    cairo_stroke(cr);

    //Draw bg
    for(int i = 0; i < numSides; ++i) {
        HSVtoRGB(Hue(), Sat(), 0.8, colour);
        cairo_set_source_rgb(cr, colour[0], colour[1], colour[2]);

        const double in = hexagonRadius - 0.1 + 0.05 * sin(time * 5);
        DrawSideShape(cr, i, numSides, in, hexagonRadius);
        cairo_fill(cr);

        HSVtoRGB(Hue(), Sat(), 0.2, colour);
        cairo_set_source_rgb(cr, colour[0], colour[1], colour[2]);
        DrawSideShape(cr, i, numSides, 0, in);
        cairo_fill(cr);

        HSVtoRGB(Hue(), Sat(), i % 2 ? 0.2 : 0.3, colour);
        cairo_set_source_rgb(cr, colour[0], colour[1], colour[2]);
        DrawSideShape(cr, i, numSides, hexagonRadius, 32);
        cairo_fill(cr);
    }

    //Draw obstacles
    HSVtoRGB(Hue(), Sat(), 0.8, colour);
    cairo_set_source_rgb(cr, colour[0], colour[1], colour[2]);

    for(int i = 0; i < numSides; i++) {
        Obstacle* obs = m_model->GetObstacle(i);

        while(obs) {
            const double pd = m_model->GetPlayerDistance();
            double start = obs->start - pd + playerRadius;
            const double end = start + obs->end - obs->start;

            if(start < hexagonRadius) {
                start = hexagonRadius;
            }

            if(end > hexagonRadius) {
                DrawSideShape(cr, i, numSides, start, end);
                cairo_fill(cr);
            }

            obs = obs->next;
        }
    }

    //Draw player
    if(m_drawPlayer) {
        const double pos = m_model->GetPlayerPosition();

        //Get the two corners the player is between
        const int posMin = floor(pos);
        const int posMax = ceil(pos);

        const double posMinX = playerRadius * cos(posMin * 2 * M_PI / numSides);
        const double posMinY = playerRadius * sin(posMin * 2 * M_PI / numSides);
        const double posMaxX = playerRadius * cos(posMax * 2 * M_PI / numSides);
        const double posMaxY = playerRadius * sin(posMax * 2 * M_PI / numSides);

        const double lerp = pos - posMin;

        const double posX = LInterp(lerp, posMinX, posMaxX);
        const double posY = LInterp(lerp, posMinY, posMaxY);

        const double pulseScale = 0.1 + 0.025 * sin(time * 10);
        const double turnMod = m_model->IsGameOver() ? 0 : m_model->GetPlayerDirection() * 15;

        HSVtoRGB(Hue(), Sat(), 1, colour);
        cairo_set_source_rgb(cr, colour[0], colour[1], colour[2]);

        cairo_save(cr);
        cairo_translate(cr, posX, posY);
        cairo_rotate(cr, pos * 2 * M_PI / numSides);

        cairo_move_to(cr, -0.2, -pulseScale);
        cairo_line_to(cr, 0, 0);
        cairo_line_to(cr, -0.2, pulseScale);
        cairo_close_path(cr);
        cairo_fill(cr);

        cairo_restore(cr);
    }
}

const double HexagonView::Hue() const
{
    if(m_model) {
        const double hueDiff = m_hueRange * sin(m_model->GetTime() * m_hueSpeed);
        const double hue = m_baseHue + hueDiff;
        return Cycle(hue, 0, 1);
    } else {
        return m_baseHue;
    }
}

const double HexagonView::Sat() const
{
    return 1.0;
}

void HexagonView::SetBaseHue(const double hue)
{
    m_baseHue = Clamp(hue, 0, 1);
}

void HexagonView::SetHueRange(const double range)
{
    m_hueRange = range;
}

void HexagonView::SetHueSpeed(const double speed)
{
    m_hueSpeed = speed;
}

void HexagonView::SetDrawPlayer(bool draw)
{
    m_drawPlayer = draw;
}
