#pragma once

#define MAX_SIDES 8

struct Obstacle;

class HexagonModel
{
public:
    HexagonModel(const int maxSides = 16);
    ~HexagonModel();

    void            Simulate(const double dt);
    void            SetPlayerDirection(const int direction);
    void            SetGameSpeed(const double speed);
    void            SetNumSides(const int numSides);

    Obstacle*       GetObstacle(const int side)                 const;
    const int       GetNumSides()                               const;
    const double    GetPlayerPosition()                         const;
    const double    GetPlayerDistance()                         const;
    const double    GetTime()                                   const;
    const bool      IsGameOver()                                const;

private:
    void            Tick(const double dt);
    void            AddObstacle();

    const bool      WillCollide(const double position)          const;

    double          m_gameSpeed;

    double          m_curTime;

    double          m_playerPosition;   //Position from 0..m_numSides
    double          m_playerSpeed;      //Speed player can move sideways
    double          m_curDistance;      //Current distance through obstacles
    double          m_obsDistance;      //Distance to end of last obstacle
    double          m_obsSpeed;         //Speed obstacles approach at
    double          m_timeDue;          //Time left to simulate 

    bool            m_gameOver;
    int             m_playerDirection;

    const int       m_maxSides;
    int             m_numSides;

    Obstacle**      m_obstacles;

};

struct Obstacle
{
    double          start;
    double          end;
    Obstacle*       next;
};

struct Pattern
{
    int             numObstacles[6];
    Obstacle        obstacles[6][10];
};
