#include "Game.hpp"

#include "HexagonView.hpp"

#include <SDL2/SDL.h>

#include <iostream>
#include <time.h>

Game::Game()
{
    m_model = 0;
    m_window = 0;
    m_surface = 0;
}

Game::~Game()
{
    if(m_model)
        delete m_model;
}

void Game::Run()
{

    if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        std::cout << SDL_GetError() << std::endl;
        return;
    }

    m_window = SDL_CreateWindow("Hexagon", SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);

    m_surface = SDL_CreateRGBSurface(0, 1280, 720, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0);

    if(!m_window) {
        std::cout << SDL_GetError() << std::endl;
        return;
    }

    m_model = new HexagonModel();

    HexagonView view;
    view.SetModel(m_model);
    view.SetBaseHue(0.3);
    view.SetHueRange(0.3);
    view.SetHueSpeed(0.5);

    bool paused = false;
    bool running = true;

    timespec timespec;
    clock_gettime(CLOCK_MONOTONIC, &timespec);

    double prevTime = timespec.tv_sec + timespec.tv_nsec * 0.000000001;
    while(running)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {

            if(e.type == SDL_QUIT) {
                running = false;
            }

            if(e.type == SDL_KEYDOWN)
            {
                if(e.key.keysym.sym == SDLK_SPACE) {
                    paused = !paused;
                }

                if(e.key.keysym.sym == SDLK_r) {
                    delete m_model;
                    m_model = new HexagonModel();
                    view.SetModel(m_model);
                }

            }

        }

        const Uint8* keyboard = SDL_GetKeyboardState(NULL);

        if(keyboard[SDL_SCANCODE_ESCAPE]) {
            running = false;
        }
        
        {
            int dir = 0;

            if(keyboard[SDL_SCANCODE_RIGHT] ||
               keyboard[SDL_SCANCODE_D]) {
                dir += 1;
            }

            if(keyboard[SDL_SCANCODE_LEFT] ||
               keyboard[SDL_SCANCODE_A]) {
                dir -= 1;
            }

            m_model->SetPlayerDirection(dir);
        }


        if(keyboard[SDL_SCANCODE_UP] ||
           keyboard[SDL_SCANCODE_W]) {
            m_model->SetGameSpeed(2.0);
        } else if(keyboard[SDL_SCANCODE_DOWN] ||
                  keyboard[SDL_SCANCODE_S]) {
            m_model->SetGameSpeed(0.2);
        } else {
            m_model->SetGameSpeed(1.0);
        }

        clock_gettime(CLOCK_MONOTONIC, &timespec);
        double curTime = timespec.tv_sec + timespec.tv_nsec * 0.000000001;

        double dt = curTime - prevTime;
        prevTime = curTime;

        //More than a second? We must be debugging.
        if(dt > 1.0) {
            dt = 0.01;
        }

        if(!paused) {
            m_model->Simulate(dt);
        }


        //Draw to our surface
        view.Draw(m_surface);

        //Copy our surface to window surface
        {
            SDL_Rect r = {0,0,1280,720};
            SDL_BlitSurface(m_surface, &r, SDL_GetWindowSurface(m_window), &r);
        }

        //Display window surface
        SDL_UpdateWindowSurface(m_window);
    }
}

