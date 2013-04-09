#include "HexagonModel.hpp"

#include <stdlib.h>
#include <time.h>

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

    while(!IsGameOver() && m_timeDue >= tickSize)
    {
        Tick(tickSize * m_gameSpeed);
        m_timeDue -= tickSize;
        m_curTime += tickSize * m_gameSpeed;
    }
}

void HexagonModel::Tick(const double dt)
{
    //Move player sideways
    double newPlayerPos = (m_playerPosition + m_playerDirection * m_playerSpeed * m_numSides* dt);

    while(newPlayerPos >= m_numSides) {
        newPlayerPos -= m_numSides;
    }

    while(newPlayerPos < 0) {
        newPlayerPos += m_numSides;
    }

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
        AddObstacle();
    }
}

void HexagonModel::AddObstacle()
{
    const int side = rand() % m_numSides;
    
    Obstacle* obs = GetObstacle(side);
    Obstacle* lastObs = 0;

    while(obs) {
        lastObs = obs;
        obs = obs->next;
    }

    Obstacle* newObs = new Obstacle;
    newObs->start = m_obsDistance;
    newObs->end = m_obsDistance + 0.5;
    newObs->next = 0;

    if(lastObs) {
        lastObs->next = newObs;
    } else {
        m_obstacles[side] = newObs;
    }

    m_obsDistance += 1 + rand() % 2;
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
    if(numSides >= m_maxSides) {
        m_numSides = m_maxSides;
    } else if(numSides >= 3) {
        m_numSides = numSides;
    } else {
        m_numSides = 3;
    }
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

