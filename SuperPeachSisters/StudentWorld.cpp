#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include "Level.h"
#include <iostream>
#include "Actor.h"
#include <iomanip> 
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath), m_levelDone(false), m_gameDone(false), m_peach(nullptr)
{
}
StudentWorld::~StudentWorld()
{
    cleanUp();
}
int StudentWorld::init()
{
    // Reads in file name in format "levelXX.txt"
    ostringstream oss;
    oss.fill('0');
    oss << "level" << setw(2) << getLevel() << ".txt";

    Level lev(assetPath());

    string level_file = oss.str();
    Level::LoadResult result = lev.loadLevel(level_file);

    // Returns error if Level did not load in correctly
    if (result == Level::load_fail_file_not_found || result == Level::load_fail_bad_format) return GWSTATUS_LEVEL_ERROR;

    // Iterates over all values in the file
    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            Level::GridEntry ge;
            ge = lev.getContentsOf(i, j); // Reads the actor at point (i, j)
            switch (ge) // Checks what actor is at each point and spawns them into the world
            {
            case Level::peach:
                m_peach = new Peach(SPRITE_WIDTH * i, SPRITE_HEIGHT * j, this);
                break;
            case Level::pipe:
                m_actors.push_back(new Pipe(SPRITE_WIDTH * i, SPRITE_HEIGHT * j, this));
                break;
            case Level::block:
                m_actors.push_back(new Block(SPRITE_WIDTH * i, SPRITE_HEIGHT * j, this, 0));
                break;
            case Level::star_goodie_block:
                m_actors.push_back(new Block(SPRITE_WIDTH * i, SPRITE_HEIGHT * j, this, 1));
                break;
            case Level::mushroom_goodie_block:
                m_actors.push_back(new Block(SPRITE_WIDTH * i, SPRITE_HEIGHT * j, this, 2));
                break;
            case Level::flower_goodie_block:
                m_actors.push_back(new Block(SPRITE_WIDTH * i, SPRITE_HEIGHT * j, this, 3));
                break;
            case Level::flag:
                m_actors.push_back(new Flag(SPRITE_WIDTH * i, SPRITE_HEIGHT * j, this));
                break;
            case Level::mario:
                m_actors.push_back(new Mario(SPRITE_WIDTH * i, SPRITE_HEIGHT * j, this));
                break;
            case Level::goomba:
                m_actors.push_back(new Goomba(SPRITE_WIDTH * i, SPRITE_HEIGHT * j, this));
                break;
            case Level::koopa:
                m_actors.push_back(new Koopa(SPRITE_WIDTH * i, SPRITE_HEIGHT * j, this));
                break;
            case Level::piranha:
                m_actors.push_back(new Piranha(SPRITE_WIDTH * i, SPRITE_HEIGHT * j, this));
                break;
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME; // If all is well continue the game
}

// Move function is called every tick
int StudentWorld::move()
{

    m_peach->doSomething(); // Lets peach do something

    // Iterates over all other actors
    for (size_t i = 0; i < m_actors.size(); i++)
    {

        m_actors[i]->doSomething(); // Lets each actor do something
        if (m_levelDone) // Checks if flag was reached, if so end level
        {
            playSound(SOUND_FINISHED_LEVEL);
            m_levelDone = false;
            return GWSTATUS_FINISHED_LEVEL;
        }
        if (m_gameDone) // Checks if mario was reached, if so end game
        {
            playSound(SOUND_GAME_OVER);
            m_gameDone = false;
            return GWSTATUS_PLAYER_WON;
        }
        if (!m_peach->isAlive()) // Checks if peach is alive, if not decrease lives and indicate death
        {
            decLives();
            playSound(SOUND_PLAYER_DIE);
            return GWSTATUS_PLAYER_DIED;
        }
    }
    
    // Creates stats text for top of screen
    ostringstream oss;
    oss.fill('0');
    oss << "Lives: " << getLives() << " Level: " << getLevel() << " Points: " << setw(5) << getScore();

    // Updates Peach's powers after end of tick
    if (m_peach->hasFlower()) oss << " FlowerPower! ";
    if (m_peach->hasStar()) oss << " StarPower! ";
    if (m_peach->hasJump()) oss << " JumpPower! ";

    setGameStatText(oss.str());

    // Deletes all dead actors
    int total = m_actors.size();
    for (int j = total - 1; j >=0; j--)
    {
        if (!m_actors[j]->isAlive())
        {
            delete m_actors[j];
            m_actors.erase(m_actors.begin() + j);
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    // Iterates over all actors and deletes them for free memory
    size_t total = m_actors.size();
    delete m_peach;
    m_peach = nullptr;
    for (int i = total - 1; i >= 0; i--)
    {
        delete m_actors[i];
    }
    m_actors.clear();
    
}

// Attempts to damage any actors colliding with a sprite at (x, y)
bool StudentWorld::damageAt(double x, double y)
{
    bool success = false; // Assumes nothing was damaged
    
    // Iterates over all actors
    for (size_t i = 0; i < m_actors.size(); i++)
    {
        double x2 = m_actors[i]->getX();
        double y2 = m_actors[i]->getY();
        if (overlap(x, y, x2, y2)) // Checks if they overlap
        {
            if (m_actors[i]->damageable() && m_actors[i]->isAlive()) // If damageable and live, damage the actor
            {
                m_actors[i]->damaged();
                success = true;
            }
        }
    }
    return success; // Return whether an actor was damaged
}

// Damages peach
void StudentWorld::damagePeach()
{
    m_peach->damaged();
}

// Bonks any actor overlapping with a sprite at (x, y)
void StudentWorld::bonkAt(double x, double y)
{
    // Iterates over all actors
    for (size_t i = 0; i < m_actors.size(); i++)
    {
        double x2 = m_actors[i]->getX();
        double y2 = m_actors[i]->getY();
        if (overlap(x, y, x2, y2) && m_actors[i]->isAlive()) // Checks to see if actor is alive and overlaps
        {
            m_actors[i]->bonk(); // Bonk!
        }

    }
}

// Bonks peach
void StudentWorld::bonkPeach()
{
    m_peach->bonk();
}

// Checks if peach overlaps with a sprite at (x, y)
bool StudentWorld::isPeachAt(double x, double y)
{
    if (overlap(m_peach->getX(), m_peach->getY(), x, y)) return true;
    return false;
}

// Checks to see if a sprite can move to (x, y)
bool StudentWorld::canMove(double x, double y)
{
    // Iterates over all actors
    for (size_t i = 0; i < m_actors.size(); i++)
    {
        double x2 = m_actors[i]->getX();
        double y2 = m_actors[i]->getY();
        if (overlap(x, y, x2, y2)) // Checks to see if they overlap
        {
            if (m_actors[i]->stop()) return false; // If this actor prevents movement the sprite can not move here
        }

    }
    return true;
}

// Checks to see if a single pixel is overlapped by a structure
bool StudentWorld::canMovePixel(double x, double y)
{
    // Iterates over all actors
    for (size_t i = 0; i < m_actors.size(); i++)
    {
        double x2 = m_actors[i]->getX();
        double y2 = m_actors[i]->getY();
        if (x >= x2 && x <= x2 + SPRITE_WIDTH - 1 && y >= y2 && y <= y2 + SPRITE_HEIGHT - 1) // Checks to see if there is overlap
        {
            if (m_actors[i]->stop()) return false; // If this actor prevents movement the sprite can not move here
        }

    }
    return true;
}

// Checks if a sprite at (x1, y1) overlaps with a sprite at (x2, y2)
bool StudentWorld::overlap(double x1, double y1, double x2, double y2)
{
    // Checks if the bottom left corner is in the sprite
    if (x1 <= x2 + SPRITE_WIDTH - 1 && x1 >= x2 && y1 <= y2 + SPRITE_HEIGHT - 1 && y1 >= y2) return true;
    // Checks if the bottom right corner is in the sprite
    if (x1 + SPRITE_WIDTH - 1 <= x2 + SPRITE_WIDTH - 1 && x1 + SPRITE_WIDTH - 1 >= x2 && y1 <= y2 + SPRITE_HEIGHT - 1 && y1 >= y2) return true;
    // Checks if the top left corner is in the sprite
    if (x1 <= x2 + SPRITE_WIDTH - 1 && x1 >= x2 && y1 + SPRITE_HEIGHT - 1 <= y2 + SPRITE_HEIGHT - 1 && y1 + SPRITE_HEIGHT - 1 >= y2) return true;
    // Checks if the top right corner is in the sprite
    if (x1 + SPRITE_WIDTH - 1 <= x2 + SPRITE_WIDTH - 1 && x1 + SPRITE_WIDTH - 1 >= x2 && y1 + SPRITE_HEIGHT - 1 <= y2 + SPRITE_HEIGHT - 1 && y1 + SPRITE_HEIGHT - 1 >= y2) return true;
    return false;
}

// Spawns a new peach fireball
void StudentWorld::addPeachFireball(int x, int y, int dir)
{
    m_actors.push_back(new PeachFireball(x, y, dir, this));
}

// Spawns a new piranha fireball
void StudentWorld::addPiranhaFireball(int x, int y, int dir)
{
    m_actors.push_back(new PiranhaFireball(x, y, dir, this));
}

// Spawns a new shell
void StudentWorld::addShell(int x, int y, int dir)
{
    m_actors.push_back(new Shell(x, y, dir, this));
}

// Spawns a new flower
void StudentWorld::addFlower(int x, int y)
{
    m_actors.push_back(new Flower(x, y, this));
}

// Spawns a new star
void StudentWorld::addStar(int x, int y)
{
    m_actors.push_back(new Star(x, y, this));
}

// Spawns a new shroom
void StudentWorld::addShroom(int x, int y)
{
    m_actors.push_back(new Mushroom(x, y, this));
}

// Marks the level as done
void StudentWorld::levelDone()
{
    m_levelDone = true;
}

// Marks the game as done
void StudentWorld::gameDone()
{
    m_gameDone = true;
}

// Gives peach star power
void StudentWorld::giveStar()
{
    m_peach->getStarPower();
}

// Gives peach flower power
void StudentWorld::giveFlower()
{
    m_peach->getFlowerPower();
}

// Gives peach jump power
void StudentWorld::giveJump()
{
    m_peach->getJumpPower();
}

// Sets peach's health to the argument value
void StudentWorld::setPeachHealth(int health)
{
    m_peach->changeHealth(health - m_peach->getHealth());
}

// Checks if peach has star
bool StudentWorld::doesPeachStar()
{
    return m_peach->hasStar();
}

// Checks if peach is in range of the y value
bool StudentWorld::peachIsInSight(double y)
{
    // Returns true if peach is within 1.5*SPRITE_HEIGHT
    return(m_peach->getY() <= y + 1.5 * SPRITE_HEIGHT && m_peach->getY() >= y - 1.5 * SPRITE_HEIGHT); 
}

// Checks if peach is to the left of the x value
bool StudentWorld::peachIsLeft(double x)
{
    // Returns true if peach's x is less than the x value
    return(m_peach->getX() < x);
}

// Checks if peach is close by
bool StudentWorld::peachIsClose(double x)
{
    // Returns true if peach is within 8 sprites of the x value
    return(m_peach->getX() < x + 8 * SPRITE_WIDTH && m_peach->getX() > x - 8 * SPRITE_WIDTH);
}