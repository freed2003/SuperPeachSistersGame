#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include "Actor.h"
#include <sstream>
#include <vector>
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Peach;
class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void bonkAt(double x, double y);
	void bonkPeach();
	bool isPeachAt(double x, double y);
	bool canMove(double x, double y);
	bool canMovePixel(double x, double y);
	void levelDone();
	void gameDone();
	bool damageAt(double x, double y);
	bool peachIsInSight(double y);
	bool peachIsLeft(double x);
	bool peachIsClose(double x);
	void damagePeach();
	bool doesPeachStar();
	void addPeachFireball(int x, int y, int dir);
	void addPiranhaFireball(int x, int y, int dir);
	void addShell(int x, int y, int dir);
	void addFlower(int x, int y);
	void addStar(int x, int y);
	void addShroom(int x, int y);
	void giveStar();
	void giveFlower();
	void giveJump();
	void setPeachHealth(int health);
private:
	bool overlap(double x1, double y1, double x2, double y2);
	std::vector<Actor*> m_actors;
	bool m_levelDone;
	bool m_gameDone;
	Peach* m_peach;
};

#endif // STUDENTWORLD_H_