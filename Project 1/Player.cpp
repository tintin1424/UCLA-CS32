#include "Player.h"
#include "Arena.h"

#include <iostream>
using namespace std;

///////////////////////////////////////////////////////////////////////////
//  Player implementations
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be in some Arena!" << endl;
        exit(1);
    }
    if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
            << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_age = 0;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

int Player::age() const
{
    return m_age;
}

string Player::takeComputerChosenTurn()
{
    // How dangerous is it to stand?
    int standDanger = computeDanger(m_row, m_col);

    // if it's not safe, see if moving is safer
    if (standDanger > 0)
    {
        int bestMoveDanger = standDanger;
        int bestMoveDir = UP;  // arbitrary initialization

          // check the four directions to see if any move is
          // better than standing, and if so, record the best
        if (m_row > 1)
        {
            int d = computeDanger(m_row - 1, m_col);
            if (d < bestMoveDanger)
            {
                bestMoveDanger = d;
                bestMoveDir = UP;
            }
        }
        if (m_row < m_arena->rows())
        {
            int d = computeDanger(m_row + 1, m_col);
            if (d < bestMoveDanger)
            {
                bestMoveDanger = d;
                bestMoveDir = DOWN;
            }
        }
        if (m_col > 1)
        {
            int d = computeDanger(m_row, m_col - 1);
            if (d < bestMoveDanger)
            {
                bestMoveDanger = d;
                bestMoveDir = LEFT;
            }
        }
        if (m_col < m_arena->cols())
        {
            int d = computeDanger(m_row, m_col + 1);
            if (d < bestMoveDanger)
            {
                bestMoveDanger = d;
                bestMoveDir = RIGHT;
            }
        }

        // if moving is better, move
        if (bestMoveDanger < standDanger)
        {
            move(bestMoveDir);
            return "Moved.";
        }
    }

    // If we're not going to move, we may as well shoot at the nearest
    // robot.  Search the four directions at increasing distances for
    // a robot.
    bool shot = false;
    bool shotSuccess = false;
    for (int k = 1; k < m_arena->rows() || k < m_arena->cols(); k++)
    {
        if (m_row - k >= 1 && m_arena->nRobotsAt(m_row - k, m_col) > 0)
        {
            shotSuccess = shoot(UP);
            shot = true;
            break;
        }
        if (m_row + k <= m_arena->rows() && m_arena->nRobotsAt(m_row + k, m_col) > 0)
        {
            shotSuccess = shoot(DOWN);
            shot = true;
            break;
        }
        if (m_col - k >= 1 && m_arena->nRobotsAt(m_row, m_col - k) > 0)
        {
            shotSuccess = shoot(LEFT);
            shot = true;
            break;
        }
        if (m_col + k <= m_arena->cols() && m_arena->nRobotsAt(m_row, m_col + k) > 0)
        {
            shotSuccess = shoot(RIGHT);
            shot = true;
            break;
        }
    }
    if (shot)
    {
        if (shotSuccess)
            return "Shot and hit!";
        else
            return "Shot and missed!";
    }

    // No robots to shoot.  Just stand, then.
    stand();
    return "Stood.";
}

void Player::stand()
{
    m_age++;
    m_arena->thePrevious().dropACrumb(m_row, m_col);
}

void Player::move(int dir)
{
    m_age++;
    switch (dir)
    {
    case UP:     if (m_row > 1)               m_row--; break;
    case DOWN:   if (m_row < m_arena->rows()) m_row++; break;
    case LEFT:   if (m_col > 1)               m_col--; break;
    case RIGHT:  if (m_col < m_arena->cols()) m_col++; break;
    }

    m_arena->thePrevious().dropACrumb(m_row, m_col);
}

bool Player::shoot(int dir)
{
    m_age++;
    m_arena->thePrevious().dropACrumb(m_row, m_col);

    if (rand() % 3 == 0)  // miss with 1/3 probability
        return false;

    // determine row and column delta for the direction
    int rDelta = 0;
    int cDelta = 0;
    switch (dir)
    {
    case UP:     rDelta = -1; break;
    case DOWN:   rDelta = +1; break;
    case LEFT:   cDelta = -1; break;
    case RIGHT:  cDelta = +1; break;
    default:     return false;  // Bad direction!
    }

    // check along the direction until we find a robot
    // or encounter the edge
    int r = row();
    int c = col();
    int distance = 0;
    do
    {
        if (m_arena->nRobotsAt(r, c) > 0)
        {
            m_arena->damageRobotAt(r, c);  // The shot damages one robot
            return true;
        }
        r += rDelta;
        c += cDelta;
        distance++;
    } while (r >= 1 && r <= m_arena->rows() &&
        c >= 1 && c <= m_arena->cols() && distance <= MAXSHOTLEN);
    return false;
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}

int Player::computeDanger(int r, int c) const
{
    // danger is the number of robots who might move to
    // position r,c.  Notice that if r,c is at an edge,
    // then a robot at that position is dangerous,
    // because it might not be able to move off that spot
    // at its turn.

    int danger = 0;

    if (r > 1)
        danger += m_arena->nRobotsAt(r - 1, c);
    else
        danger += m_arena->nRobotsAt(r, c);

    if (r < m_arena->rows())
        danger += m_arena->nRobotsAt(r + 1, c);
    else
        danger += m_arena->nRobotsAt(r, c);

    if (c > 1)
        danger += m_arena->nRobotsAt(r, c - 1);
    else
        danger += m_arena->nRobotsAt(r, c);

    if (c < m_arena->cols())
        danger += m_arena->nRobotsAt(r, c + 1);
    else
        danger += m_arena->nRobotsAt(r, c);

    return danger;
}