#include "Game.hpp"

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

    m_window = new sf::RenderWindow(sf::VideoMode(1920,1080), "Hexagon");

    bool paused = false;

    sf::Clock clock;
    while(m_window->isOpen())
    {
        sf::Event e;
        while(m_window->pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
                m_window->close();

            if(e.type == sf::Event::KeyPressed)
            {
                if(e.key.code == sf::Keyboard::Space) {
                    paused = !paused;
                }

                if(e.key.code == sf::Keyboard::R) {
                    delete m_model;
                    m_model = new HexagonModel();
                    view.SetModel(m_model);
                }

                if(e.key.code == sf::Keyboard::P) {
                    m_model->SetNumSides(m_model->GetNumSides() + 2);
                }

                if(e.key.code == sf::Keyboard::O) {
                    m_model->SetNumSides(m_model->GetNumSides() - 2);
                }
            }

        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            m_window->close();
        }
        
        {
            int dir = 0;

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                dir += 1;

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                dir -= 1;

            m_model->SetPlayerDirection(dir);
        }


        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            m_model->SetGameSpeed(2.0);
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            m_model->SetGameSpeed(0.2);
        } else {
            m_model->SetGameSpeed(1.0);
        }

        const double dt = clock.restart().asSeconds();

        if(!paused) {
            m_model->Simulate(dt);
        }

        m_window->clear();
        view.Draw(m_window);
        m_window->display();
    }
}

