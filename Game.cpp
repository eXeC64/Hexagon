#include "Game.hpp"

#include <SDL/SDL.h>

#include "HexagonView.hpp"
#include "Util.hpp"

Game::Game()
{
    m_model = 0;
    m_window = 0;
}

Game::~Game()
{
    if(m_model)
        delete m_model;

    if(m_window)
        delete m_window;
}

void Game::Run()
{
    m_model = new HexagonModel();

    /*
    HexagonView view;
    view.SetModel(m_model);
    view.SetBaseHue(0.3);
    view.SetHueRange(0.3);
    view.SetHueSpeed(0.5);
    */

    m_window = new Win110ct(1920,1080,16);

    bool paused = false;
    bool running = true;

    double oldTime = GetTime();
    while(running)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
                running = false;

            if(e.type == SDL_KEYDOWN)
            {
                if(e.key.keysym.sym == SDLK_SPACE) {
                    paused != paused;
                }

                if(e.key.keysym.sym == SDLK_q) {
                    running = false;
                }

                if(e.key.keysym.sym == SDLK_r) {
                    delete m_model;
                    m_model = new HexagonModel();
                    //view.SetModel(m_model);
                }
            }

        }

        Uint8* keystate = SDL_GetKeyState(NULL);
        {
            int dir = 0;

            if(keystate[SDLK_LEFT] || keystate[SDLK_d]) {
                dir += 1;
            }

            if(keystate[SDLK_RIGHT] || keystate[SDLK_a]) {
                dir -= 1;
            }

            m_model->SetPlayerDirection(dir);
        }

        if(keystate[SDLK_UP] || keystate[SDLK_w]) {
            m_model->SetGameSpeed(2.0);
        } else if(keystate[SDLK_DOWN] || keystate[SDLK_s]) {
            m_model->SetGameSpeed(0.2);
        } else {
            m_model->SetGameSpeed(1.0);
        }

        const double newTime = GetTime();
        const double dt = oldTime - newTime;
        oldTime = newTime;

        if(!paused) {
            m_model->Simulate(dt);
        }

        //Clear
        m_window->clear();
        m_window->clearBack();

        //Draw
        //view.Draw(m_window);

        //Swapbuffer
        m_window->render();

    }
}

