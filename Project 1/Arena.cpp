#include "Arena.h"
#include "Player.h"
#include "Robot.h"
#include "globals.h"

#include <iostream>
using namespace std;

///////////////////////////////////////////////////////////////////////////
//  Arena implementations
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
    : m_previous(nRows, nCols)
{
    if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
            << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nRobots = 0;
}

Arena::~Arena()
{
    for (int k = 0; k < m_nRobots; k++)
        delete m_robots[k];
    delete m_player;
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::robotCount() const
{
    return m_nRobots;
}

int Arena::nRobotsAt(int r, int c) const
{
    int count = 0;
    for (int k = 0; k < m_nRobots; k++)
    {
        const Robot* rp = m_robots[k];
        if (rp->row() == r && rp->col() == c)
            count++;
    }
    return count;
}

void Arena::display(string msg) const
{
    // Position (row,col) in the arena coordinate system is represented in
    // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;

    // Fill the grid with dots
    for (r = 0; r < rows(); r++)
        for (c = 0; c < cols(); c++)
            grid[r][c] = '.';

    // Indicate each robot's position
    for (int k = 0; k < m_nRobots; k++)
    {
        const Robot* rp = m_robots[k];
        char& gridChar = grid[rp->row() - 1][rp->col() - 1];
        switch (gridChar)
        {
        case '.':  gridChar = 'R'; break;
        case 'R':  gridChar = '2'; break;
        case '9':  break;
        default:   gridChar++; break;  // '2' through '8'
        }
    }

    // Indicate player's position
    if (m_player != nullptr)
    {
        // Set the char to '@', unless there's also a robot there,
        // in which case set it to '*'.
        char& gridChar = grid[m_player->row() - 1][m_player->col() - 1];
        if (gridChar == '.')
            gridChar = '@';
        else
            gridChar = '*';
    }

    // Draw the grid
    clearScreen();
    for (r = 0; r < rows(); r++)
    {
        for (c = 0; c < cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;

    // Write message, robot, and player info
    cout << endl;
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << robotCount() << " robots remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player." << endl;
    else
    {
        if (m_player->age() > 0)
            cout << "The player has lasted " << m_player->age() << " steps." << endl;
        if (m_player->isDead())
            cout << "The player is dead." << endl;
    }
}

bool Arena::addRobot(int r, int c)
{
    // Dynamically allocate a new Robot and add it to the arena
    if (m_nRobots == MAXROBOTS)
        return false;
    m_robots[m_nRobots] = new Robot(this, r, c);
    m_nRobots++;
    return true;
}

bool Arena::addPlayer(int r, int c)
{
    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

    // Dynamically allocate a new Player and add it to the arena
    m_player = new Player(this, r, c);
    return true;
}

void Arena::damageRobotAt(int r, int c)
{
    int k = 0;
    for (; k < m_nRobots; k++)
    {
        if (m_robots[k]->row() == r && m_robots[k]->col() == c)
            break;
    }
    if (k < m_nRobots && !m_robots[k]->takeDamageAndLive())  // robot dies
    {
        delete m_robots[k];
        m_robots[k] = m_robots[m_nRobots - 1];
        m_nRobots--;
    }
}

bool Arena::moveRobots()
{
    for (int k = 0; k < m_nRobots; k++)
    {
        Robot* rp = m_robots[k];
        rp->move();
        if (rp->row() == m_player->row() && rp->col() == m_player->col())
            m_player->setDead();
    }

    // return true if the player is still alive, false otherwise
    return !m_player->isDead();
}

// PREVIOUS
Previous& Arena::thePrevious()
{
    return m_previous;
}