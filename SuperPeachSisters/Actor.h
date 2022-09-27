#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include "GameWorld.h"
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class Actor : public GraphObject
{
public:
	Actor(int ImageID, int startX, int startY, int startDirection, int depth, double size, StudentWorld* world);
	virtual ~Actor();
	virtual void doSomething() = 0;
	virtual void bonk() = 0;
	virtual bool stop();
	virtual void damaged() = 0;
	void changeHealth(int dmg);
	int getHealth();
	virtual bool damageable();
	bool isAlive();
private:
	int m_health;
	StudentWorld* m_world;
	bool m_alive;
protected:
	StudentWorld* getWorld();
	void changeDirection();
	void setLiving(bool status);
	
};

class Peach : public Actor
{
public:
	Peach(int startX, int startY, StudentWorld* world);
	virtual ~Peach();
	virtual bool damageable();
	virtual void damaged();
	virtual void doSomething();
	virtual void bonk();
	void getStarPower();
	void getFlowerPower();
	void getJumpPower();
	bool hasStar();
	bool hasJump();
	bool hasFlower();
private:
	int m_star;
	int m_tempInv;
	int m_recharge;
	int m_jump;
	bool m_jpower;
	bool m_flower;
};

class Structure : public Actor
{
public:
	Structure(int ImageID, int startX, int startY, StudentWorld* world);
	virtual ~Structure() {}
	virtual bool stop();
	virtual void bonk();
	virtual void doSomething();
	virtual void damaged();
};

class Block : public Structure
{
public:
	Block(int startX, int startY, StudentWorld* world, int goods);
	virtual void bonk();
private:
	int m_goods;
	bool m_full;
};

class Pipe : public Structure
{
public:
	Pipe(int startX, int startY, StudentWorld* world);
private:
};

class Projectile : public Actor
{
public:
	Projectile(int ImageID, int startX, int startY, int dir, StudentWorld* world);
	virtual ~Projectile() {}
	virtual void doSomething();
	virtual void bonk();
	virtual void damaged() {}
private:
	virtual void damageTime();
};

class GoodProj : public Projectile
{
public:
	GoodProj(int ImageID, int startX, int startY, int dir, StudentWorld* world);
	virtual ~GoodProj() {}
private:
	virtual void damageTime();
};

class Shell : public GoodProj
{
public:
	Shell(int startX, int startY, int dir, StudentWorld* world);
};

class PiranhaFireball : public Projectile
{
public:
	PiranhaFireball(int startX, int startY, int dir, StudentWorld* world);
};

class PeachFireball : public GoodProj
{
public:
	PeachFireball(int startX, int startY, int dir, StudentWorld* world);
};

class LevelEnder : public Actor
{
public:
	LevelEnder(int ImageID, int startX, int startY, StudentWorld* world);
	virtual void doSomething();
	virtual void bonk();
	virtual void damaged() {}
	virtual ~LevelEnder() {}
private:
	virtual void whatHappensNext() = 0;
};

class Flag : public LevelEnder
{
public:
	Flag(int startX, int startY, StudentWorld* world);
private:
	virtual void whatHappensNext();
};

class Mario : public LevelEnder
{
public:
	Mario(int startX, int startY, StudentWorld* world);
private:
	virtual void whatHappensNext();
};

class Mob : public Actor
{
public:
	Mob(int ImageID, int startX, int startY, StudentWorld* world);\
	virtual ~Mob() {}
	virtual void doSomething();
	virtual void bonk();
	virtual void damaged();
	virtual bool damageable();
private:
	virtual void forKoopa() {}
};

class Goomba : public Mob
{
public:
	Goomba(int startX, int startY, StudentWorld* world);
};

class Piranha : public Mob
{
public:
	Piranha(int startX, int startY, StudentWorld* world);
	virtual void doSomething();
private:
	int m_delay;
};

class Koopa : public Mob
{
public:
	Koopa(int startX, int startY, StudentWorld* world);
private:
	virtual void forKoopa();
};

class Goodie : public Actor
{
public:
	Goodie(int ImageID, int startX, int startY, StudentWorld* world);
	virtual ~Goodie() {}
	virtual void doSomething();
	virtual void bonk();
	virtual void damaged() {}
private:
	virtual void upgrade() = 0;
};

class Star : public Goodie
{
public:
	Star(int startX, int startY, StudentWorld* world);
private:
	virtual void upgrade();
};

class Flower : public Goodie
{
public:
	Flower(int startX, int startY, StudentWorld* world);
private:
	virtual void upgrade();
};

class Mushroom : public Goodie
{
public:
	Mushroom(int startX, int startY, StudentWorld* world);
private:
	virtual void upgrade();
};
#endif // ACTOR_H_