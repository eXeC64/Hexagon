#include "HexagonModel.hpp"

#include <stdlib.h>
#include <time.h>

#include "Util.hpp"

HexagonModel::HexagonModel(const int maxSides) : m_maxSides(maxSides)
{
    m_gameSpeed = 1.0;

    m_playerPosition = 0.5;
    m_playerSpeed = 1.2;
    m_playerDirection = 0;

    m_curDistance = 0;
    m_obsDistance = 8;
    m_obsSpeed = 6;

    m_curTime = 0;
    m_timeDue = 0;

    m_rotation = 0;
    m_rotationSpeed = 0;
    m_nextRotationSpeedChange = 0;

    m_invincible = false;
    m_gameOver = false;

    m_numSides = 6;

    m_lastPattern = -1;

    m_obstacles = new Obstacle*[m_maxSides];

    for(int i = 0; i < m_maxSides; ++i) {
        m_obstacles[i] = 0;
    }

    srand(time(0));
}

HexagonModel::~HexagonModel()
{
    for(int i = 0; i < m_maxSides; ++i) {
        if(m_obstacles[i] != 0) {
            delete m_obstacles[i];
        }
    }

    delete[] m_obstacles;
}

void HexagonModel::Simulate(const double dt)
{
    m_timeDue += dt;

    const double tickSize = 1/100.0;

    while(!IsGameOver() && m_timeDue >= tickSize) {
        Tick(tickSize * m_gameSpeed);
        m_timeDue -= tickSize;
        m_curTime += tickSize * m_gameSpeed;
    }
}

void HexagonModel::Tick(const double dt)
{
    //Rotate game
    if(m_curTime >= m_nextRotationSpeedChange) {
        m_nextRotationSpeedChange = m_curTime + 5 + rand() % 10;
        m_rotationSpeed = (60 + rand() % 60) * 0.03;
        if(rand() % 2) {
            m_rotationSpeed = -m_rotationSpeed;
        }
    }

    m_rotation += m_rotationSpeed * dt;


    //Move player sideways
    const double playerPosDelta = (m_playerDirection * m_playerSpeed * m_numSides * dt);
    const double newPlayerPos = Cycle(m_playerPosition + playerPosDelta, 0, m_numSides);

    if(!WillCollide(newPlayerPos)) {
        m_playerPosition = newPlayerPos;
    }


    //Move forward
    m_curDistance += m_obsSpeed * dt;

    //Check for collisions
    if(!m_invincible && WillCollide(m_playerPosition)) {
        m_gameOver = true;
    }

    //Delete old obstacles
    for(int i = 0; i < m_maxSides; ++i) {
        Obstacle* obs = m_obstacles[i];

        if(obs && obs->end < m_curDistance - 1) {
            m_obstacles[i] = obs->next;
            delete obs;
        }
    }

    //Add new obstacles if needed
    while(m_obsDistance - m_curDistance < 10) {
        AddPattern();
    }
}

void HexagonModel::AddObstacle(const int side, const double start, const double length)
{
    Obstacle* obs = GetObstacle(side);
    Obstacle* lastObs = 0;

    while(obs) {
        lastObs = obs;
        obs = obs->next;
    }

    Obstacle* newObs = new Obstacle;
    newObs->start = start;
    newObs->end = start + length;
    newObs->next = 0;

    if(lastObs) {
        lastObs->next = newObs;
    } else {
        m_obstacles[side] = newObs;
    }

    m_obsDistance = Max(m_obsDistance, start + length);
}

void HexagonModel::AddPattern()
{
    const double base = m_obsDistance + 2;

    //Direction to spin the pattern in
    const int direction = rand() % 2;

    if(m_numSides == 6) {

        //Never repeat the same pattern consecutively
        int pattern = m_lastPattern;
        while(pattern == m_lastPattern) {
            pattern = rand() % 7;
        }
        m_lastPattern = pattern;

        if(pattern == 0) {

            //Opposite hexagons
            const int iterations = 1 + rand() % 5;
            int lastSide = -1;

            //Add each hexagon
            for(int i = 0; i < iterations; ++i) {

                //Never put the gap on the same side consecutively
                int side = lastSide;
                while(side == lastSide) {
                    side = rand() % 6;
                }
                lastSide = side;

                //Construct the hexagon
                for(int s = 0; s < 6; ++s) {
                    if(s != side) {
                        AddObstacle(s, 1 + base + i * 3, 0.5);
                    }
                }
            }

        } else if(pattern == 1) {
            //Spiral
            const int size = 12 + rand() % 6;
            for(int i = 0; i < size; ++i) {
                AddObstacle(Twist(direction, i % 6, m_numSides), base + i, 1);
            }

        } else if(pattern == 2) {
            //Bullet hell

            const int iterations = 1 + rand() % 20;

            for(int i = 0; i < iterations; ++i) {

                for(int s = 0; s < 6; ++s) {
                    if((i + s) % 2) {
                        AddObstacle(s, base + i * 2, 0.5);
                    }
                }

            }
        } else if(pattern == 3) {

            //Oscilating spiral
            const int iterations = 2 + rand() % 2;

            for(int i = 0; i < iterations; ++i ) {

                AddObstacle(1, base + i * 10, 1);
                AddObstacle(2, base + i * 10, 1);
                AddObstacle(3, base + i * 10, 1);
                AddObstacle(4, base + i * 10, 1);

                AddObstacle(0, base + i * 10 + 5, 1);
                AddObstacle(1, base + i * 10 + 5, 1);
                AddObstacle(2, base + i * 10 + 5, 1);
                AddObstacle(3, base + i * 10 + 5, 1);

                AddObstacle(5, base + i * 10, 10);
            }


        } else if(pattern == 4) {
            //Braces
            const int iterations = 1 + rand() % 20;
            for(int i = 0; i < iterations; ++i) {
                int mod = i % 3;

                if(direction) {
                    mod = 2 - mod;
                }

                if(mod == 0) {
                    AddObstacle(0, base + i * 2, 0.5);
                    AddObstacle(1, base + i * 2, 0.5);
                    AddObstacle(3, base + i * 2, 0.5);
                    AddObstacle(4, base + i * 2, 0.5);
                } else if(mod == 1) {
                    AddObstacle(1, base + i * 2, 0.5);
                    AddObstacle(2, base + i * 2, 0.5);
                    AddObstacle(4, base + i * 2, 0.5);
                    AddObstacle(5, base + i * 2, 0.5);
                } else if(mod == 2) {
                    AddObstacle(2, base + i * 2, 0.5);
                    AddObstacle(3, base + i * 2, 0.5);
                    AddObstacle(5, base + i * 2, 0.5);
                    AddObstacle(0, base + i * 2, 0.5);
                }
            }
        } else if(pattern == 5) {
            //0 #######
            //1 #
            //2    #
            //3   ###
            //4    #
            //5 #
            //6 #######

            const int iterations = 2 + rand() % 3;
            for(int i = 0; i < iterations; ++i) {
                AddObstacle(0, base + i * 7, 7);
                AddObstacle(1, base + i * 7, 1);
                AddObstacle(2, base + i * 7 + 4, 1);
                AddObstacle(3, base + i * 7 + 3, 3);
                AddObstacle(4, base + i * 7 + 4, 1);
                AddObstacle(5, base + i * 7, 1);
            }
        } else if(pattern == 6) {
            //  __________
            //0   ##   ##
            //1    ##   ##
            //2 #   ##   #
            //3 ##   ##
            //4  ##   ##
            //5   ##   ##
            const int iterations = 1 + rand() % 2;
            for(int i = 0; i < iterations; ++i) {
                AddObstacle(0, base + i * 10 + 2, 2);
                AddObstacle(0, base + i * 10 + 7, 2);

                AddObstacle(1, base + i * 10 + 3, 2);
                AddObstacle(1, base + i * 10 + 8, 2);

                AddObstacle(2, base + i * 10, 1);
                AddObstacle(2, base + i * 10 + 4, 2);
                AddObstacle(2, base + i * 10 + 9, 1);

                AddObstacle(3, base + i * 10, 2);
                AddObstacle(3, base + i * 10 + 5, 2);

                AddObstacle(4, base + i * 10 + 1, 2);
                AddObstacle(4, base + i * 10 + 6, 2);

                AddObstacle(5, base + i * 10 + 2, 2);
                AddObstacle(5, base + i * 10 + 7, 2);
            }

        }
    } else {
        const int side = rand() % m_numSides;
        AddObstacle(side, m_obsDistance + 1, m_obsDistance + 1.5);
    }

}

const bool HexagonModel::WillCollide(const double position) const
{
    const int side = position;

    Obstacle* obs = GetObstacle(side);

    while(obs) {
        if(obs->start <= m_curDistance && obs->end > m_curDistance) {
            return true;
        }
        obs = obs->next;
    }

    return false;
}

void HexagonModel::SetPlayerDirection(const int direction)
{
    if(direction > 0) {
        m_playerDirection = 1;
    } else if(direction < 0) {
        m_playerDirection = -1;
    } else {
        m_playerDirection = 0;
    }
}

void HexagonModel::SetGameSpeed(const double speed)
{
    m_gameSpeed = speed;
}

void HexagonModel::SetNumSides(const int numSides)
{
    m_numSides = Clamp(numSides, 4, m_maxSides);
}

void HexagonModel::SetInvincibility(const bool invincibility)
{
    m_invincible = invincibility;
}

Obstacle* HexagonModel::GetObstacle(const int side) const
{
    if(side < 0 || side >= m_maxSides)
        return 0;

    return m_obstacles[side];
}

const int HexagonModel::GetNumSides() const
{
    return m_numSides;
}

const double HexagonModel::GetPlayerPosition() const
{
    return m_playerPosition;
}

const double HexagonModel::GetPlayerDistance() const
{
    return m_curDistance;
}

const int HexagonModel::GetPlayerDirection() const
{
    return m_playerDirection;
}

const double HexagonModel::GetTime() const
{
    return m_curTime;
}

const bool HexagonModel::IsPlayerInvincible() const
{
    return m_invincible;
}

const bool HexagonModel::IsGameOver() const
{
    return m_gameOver;
}

const double HexagonModel::GetRotation() const
{
    return m_rotation;
}
