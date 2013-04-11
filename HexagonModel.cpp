#include "HexagonModel.hpp"

#include <stdlib.h>
#include <time.h>

#include "Util.hpp"

HexagonModel::HexagonModel(const int maxSides) : m_maxSides(maxSides)
{
    m_gameSpeed = 1.0;

    m_playerPosition = 0.5;
    m_playerSpeed = 0.9;
    m_playerDirection = 0;

    m_curDistance = 0;
    m_obsDistance = 8;
    m_obsSpeed = 5;

    m_curTime = 0;
    m_timeDue = 0;

    m_gameOver = false;

    m_numSides = 6;

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
    //Move player sideways
    const double playerPosDelta = (m_playerDirection * m_playerSpeed * m_numSides * dt);
    const double newPlayerPos = Cycle(m_playerPosition + playerPosDelta, 0, m_numSides);

    if(!WillCollide(newPlayerPos)) {
        m_playerPosition = newPlayerPos;
    }


    //Move forward
    m_curDistance += m_obsSpeed * dt;

    //Check for collisions
    if(WillCollide(m_playerPosition)) {
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
    const double base = m_obsDistance;

    if(m_numSides == 6) {
        const int pattern = rand() % 6;
        switch(pattern) {
        case 0:
            //Opposite hexagons
            for(int i = base + 3; i < base + 24; i += 8) {
                AddObstacle(0, i, 0.5);
                AddObstacle(1, i, 0.5);
                AddObstacle(2, i, 0.5);
                AddObstacle(3, i, 0.5);
                AddObstacle(4, i, 0.5);

                AddObstacle(0, i + 4, 0.5);
                AddObstacle(1, i + 4, 0.5);
                AddObstacle(3, i + 4, 0.5);
                AddObstacle(4, i + 4, 0.5);
                AddObstacle(5, i + 4, 0.5);
            }
            break;
        case 1:
            //Spiral
            AddObstacle(0, base + 3, 1);
            AddObstacle(1, base + 4, 1);
            AddObstacle(2, base + 5, 1);
            AddObstacle(3, base + 6, 1);
            AddObstacle(4, base + 7, 1);
            AddObstacle(5, base + 8, 1);

            AddObstacle(0, base + 9, 1);
            AddObstacle(1, base + 10, 1);
            AddObstacle(2, base + 11, 1);
            AddObstacle(3, base + 12, 1);
            AddObstacle(4, base + 13, 1);
            AddObstacle(5, base + 14, 1);

            AddObstacle(0, base + 15, 1);
            AddObstacle(1, base + 16, 1);
            AddObstacle(2, base + 17, 1);
            AddObstacle(3, base + 18, 1);
            AddObstacle(4, base + 19, 1);
            AddObstacle(5, base + 20, 1);
            break;
        case 2:
            //Bullet hell

            for(int i = base + 3; i < base + 27; i += 9) {
                AddObstacle(0, i + 1.0, 0.5);
                AddObstacle(1, i + 3.5, 0.5);
                AddObstacle(2, i + 1.0, 0.5);
                AddObstacle(3, i + 3.5, 0.5);
                AddObstacle(4, i + 1.0, 0.5);
                AddObstacle(5, i + 3.5, 0.5);

                AddObstacle(0, i + 5.5, 0.5);
                AddObstacle(1, i + 8.0, 0.5);
                AddObstacle(2, i + 5.5, 0.5);
                AddObstacle(3, i + 8.0, 0.5);
                AddObstacle(4, i + 5.5, 0.5);
                AddObstacle(5, i + 8.0, 0.5);
            }

            break;
        case 3:
            //0    ####################
            //1 #   ######      ######   #
            //2 ##   ####   ##   ####   ##
            //3 ###   ##   ####   ##   ###
            //4 ####      ######      ####
            //5 ##########################
            AddObstacle(0, base + 6, 20);

            AddObstacle(1, base + 3, 1);
            AddObstacle(1, base + 7, 5);
            AddObstacle(1, base + 19, 5);
            AddObstacle(1, base + 28, 1);

            AddObstacle(2, base + 3, 2);
            AddObstacle(2, base + 8, 4);
            AddObstacle(2, base + 15, 2);
            AddObstacle(2, base + 20, 4);
            AddObstacle(2, base + 27, 2);

            AddObstacle(3, base + 3, 3);
            AddObstacle(3, base + 9, 2);
            AddObstacle(3, base + 14, 4);
            AddObstacle(3, base + 21, 1);
            AddObstacle(3, base + 26, 3);

            AddObstacle(4, base + 3, 4);
            AddObstacle(4, base + 13, 6);
            AddObstacle(4, base + 25, 4);

            AddObstacle(5, base + 3, 26);
            break;
        case 4:
            //Spiralling hexagons
            AddObstacle(0, base + 3, 0.5);
            AddObstacle(1, base + 3, 0.5);
            AddObstacle(2, base + 3, 0.5);
            AddObstacle(3, base + 3, 0.5);
            AddObstacle(4, base + 3, 0.5);

            AddObstacle(0, base + 6, 0.5);
            AddObstacle(1, base + 6, 0.5);
            AddObstacle(2, base + 6, 0.5);
            AddObstacle(3, base + 6, 0.5);
            AddObstacle(5, base + 6, 0.5);

            AddObstacle(0, base + 9, 0.5);
            AddObstacle(1, base + 9, 0.5);
            AddObstacle(2, base + 9, 0.5);
            AddObstacle(4, base + 9, 0.5);
            AddObstacle(5, base + 9, 0.5);

            AddObstacle(0, base + 12, 0.5);
            AddObstacle(1, base + 12, 0.5);
            AddObstacle(3, base + 12, 0.5);
            AddObstacle(4, base + 12, 0.5);
            AddObstacle(5, base + 12, 0.5);
            break;

        case 5:
            for(int i = base + 3; i < base + 15; i += 4) {
                AddObstacle(0, i, 0.5);
                AddObstacle(1, i, 0.5);
                AddObstacle(3, i, 0.5);
                AddObstacle(4, i, 0.5);

                AddObstacle(1, i + 2, 0.5);
                AddObstacle(2, i + 2, 0.5);
                AddObstacle(4, i + 2, 0.5);
                AddObstacle(5, i + 2, 0.5);
            }
            break;
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

const double HexagonModel::GetTime() const
{
    return m_curTime;
}

const bool HexagonModel::IsGameOver() const
{
    return m_gameOver;
}

