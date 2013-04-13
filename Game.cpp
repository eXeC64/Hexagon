#include "Game.hpp"

#include <sys/time.h>

#include "HexagonView.hpp"

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

    HexagonView view;
    view.SetModel(m_model);
    view.SetBaseHue(0.3);
    view.SetHueRange(0.3);
    view.SetHueSpeed(0.5);

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
                    pause != pause;
                }

                if(e.key.keysym.sym == SDLK_Q) {
                    running = false;
                }

                if(e.key.keysym.sym == SDL_R) {
                    delete m_model;
                    m_model = new HexagonModel();
                    view.SetModel(m_model);
                }
            }

        }

        Uint8* keystate = SDL_GetKeyState(NULL);
        {
            int dir = 0;

            if(keystate[SDLK_LEFT] || keystate[SDLK_D]) {
                dir += 1;
            }

            if(keystate[SDLK_RIGHT] || keystate[SDLK_A]) {
                dir -= 1;
            }

            m_model->SetPlayerDirection(dir);
        }

        if(keystate[SDLK_UP] || keystate[SDLK_W]) {
            m_model->SetGameSpeed(2.0);
        } else if(keystate[SDLK_DOWN] || keystate[SDLK_S]) {
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
        window->clear();
        window->clearBack();

        //Draw
        m_view->Draw(window);

        //Swapbuffer
        window->render();

    }
}

