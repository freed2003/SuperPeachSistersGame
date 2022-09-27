#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"
#include <cstdlib>
#include <iostream>
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
// 
Actor::Actor(int ImageID, int startX, int startY, int startDirection, int depth, double size, StudentWorld* world)
	:GraphObject(ImageID, startX, startY, startDirection, depth, size), m_world(world), m_health(100), m_alive(true)
{

}

// Adds argument value to m_health
void Actor::changeHealth(int x)
{
	m_health += x;
}


// Returns the health of the Actor
int Actor::getHealth()
{
	return m_health;
}


// Changes whether the actor is alive or dead to the argument
void Actor::setLiving(bool status)
{
	m_alive = status;
}

// Flops the direction; if started at 0, change to 180 and if start at 180 change to 0. Only works if asssuming actor only has values of 0 and 180
void Actor::changeDirection()
{
	setDirection(-1 * (getDirection() - 180));
}

Actor::~Actor()
{

}

// Damaging does nothing
void Actor::damaged()
{
	return;
}

// Set Actors to not be damageable by default
bool Actor::damageable()
{
	return false;
}

// Returns a pointer to the game world
StudentWorld* Actor::getWorld()
{
	return m_world;
}

// Checks whether the Actor is alive
bool Actor::isAlive()
{
	return m_alive;
}

// Checks if the Actor blocks movement. Set to false by default
bool Actor::stop()
{
	return false;
}

Peach::Peach(int startX, int startY, StudentWorld* world)
	:Actor(0, startX, startY, 0, 0, 1.0, world), m_tempInv(0), m_star(0), m_recharge(0), m_jump(0), m_jpower(false), m_flower(false)
{

}

Peach::~Peach()
{

}

// Peach is damageable so change to true
bool Peach::damageable()
{
	return true;
}

// Returns true if peach has jump, false otherwise
bool Peach::hasJump()
{
	return m_jpower;
}

// Returns true if peach has flower, false otherwise
bool Peach::hasFlower()
{
	return m_flower;
}

// Sets the bonk method for peach, which will be called when collided by mobs
void Peach::bonk()
{
	if (m_star) return; // If peach has the star she is invincible so leave
	changeHealth(-100); // Otherwise lose one hit point
	m_tempInv = 10; // Give yourself some temporary invicibility
	m_flower = false; // Remove flower power
	m_jpower = false; // Remove jump power
	if (getHealth()) getWorld()->playSound(SOUND_PLAYER_HURT); // Play the hurt noise if you have health
	if (getHealth() <= 0) setLiving(false); // Otherwise change status to dead

}

// Sets the doSomething method for peach, called every tick
void Peach::doSomething()
{
	if (!isAlive()) return; // Checks if peach is alive. If not, she does nothing

	int ch;
	if (m_star) m_star--; // Decrease star ticks by one
	if (m_tempInv) m_tempInv--; // Decrease temporary invincibility ticks by one
	if (m_recharge) m_recharge--; // Decrease time for fireball recharge by one
	getWorld()->bonkAt(getX(), getY()); // If peach is colliding with something bonk it
	if (m_jump) // Checks if peach is currently jumping
	{
		if (getWorld()->canMove(getX(), getY() + 4)) // Checks if peach can move upwards
		{
			moveTo(getX(), getY() + 4); // Move up 4 pixels
			m_jump--; // Decrease jump ticks by one
		}
		else // If something is blocking peach
		{
			getWorld()->bonkAt(getX(), getY() + 4); // Bonks whatever is blocking peach
			m_jump = 0; // Sets jump ticks 0 as peach will no longer be jumping
		}
	}
	else // If peach is not jumping
	{

		if (getWorld()->canMove(getX(), getY() - 4)) moveTo(getX(), getY() - 4); // Tries to fall downwards
	}
	if (getWorld()->getKey(ch)) // Checks for an input
	{
		switch (ch)
		{
		case KEY_PRESS_LEFT: 
			setDirection(180); // Changes direction to face left
			// Attempts to move to the left, otherwise bonks the blocking object
			if (getWorld()->canMove(getX() - 4, getY()))
			{
				moveTo(getX() - 4, getY());
			}
			else
			{
				getWorld()->bonkAt(getX() - 4, getY());
			}
			break;
		case KEY_PRESS_RIGHT:
			setDirection(0); // Changes direction to face right
			// Attempts to move to the right, otherwise bonks the blocking object
			if (getWorld()->canMove(getX() + 4, getY()))
			{
				moveTo(getX() + 4, getY());
			}
			else
			{
				getWorld()->bonkAt(getX() + 4, getY());
			}
			break;
		case KEY_PRESS_UP:
			if (!getWorld()->canMove(getX(), getY() - 1)) // Makes sure peach isn't floating in the air
			{
				if (m_jpower) m_jump = 12; // Sets remaining jump ticks to 12 if peach has jump power
				else m_jump = 8; // Otherwise only set to 10
				getWorld()->playSound(SOUND_PLAYER_JUMP); // Plays the jumping sound
			}
			break;
		case KEY_PRESS_SPACE:
			if (m_flower && !m_recharge) // Makes sure peach is able to fire a fireball
			{
				getWorld()->playSound(SOUND_PLAYER_FIRE); // Plays firing sound
				m_recharge = 8; // Sets recharge ticks to 8
				getWorld()->addPeachFireball(getX() + 4 * (getDirection() / (-90) + 1), getY(), getDirection()); // Spawns fireball

			}
		}
	}
}

// Peach being damaged is same as bonk
void Peach::damaged()
{
	bonk();
}

// Sets peach's star power ticks to 150
void Peach::getStarPower()
{
	m_star = 150;
	
}

// Gives paech the flower power
void Peach::getFlowerPower()
{
	m_flower = true;
}

// Gives peach the jump power
void Peach::getJumpPower()
{
	m_jpower = true;
}

// Checks if peach has star power
bool Peach::hasStar()
{
	return m_star;
}

Structure::Structure(int ImageID, int startX, int startY, StudentWorld* world)
	:Actor(ImageID, startX, startY, 0, 2, 1, world)
{

}

// Structures block movement
bool Structure::stop()
{
	return true;
}

void Structure::bonk()
{

}

void Structure::damaged()
{
	return;
}

void Structure::doSomething()
{

}

Block::Block(int startX, int startY, StudentWorld* world, int goods)
	:Structure(6, startX, startY, world), m_goods(goods), m_full(true)
{

}

// Blocks have a special bonk compared to other structures
void Block::bonk()
{
	if (!(m_full && m_goods)) // Checks if the block is empty
	{
		getWorld()->playSound(SOUND_PLAYER_BONK); // Play bonk noise
	}
	else 
	{
		// Plays sound and sets block to empty
		getWorld()->playSound(SOUND_POWERUP_APPEARS);
		m_full = false;
		// Spawns the goodies in the boock
		if (m_goods == 1) getWorld()->addStar(getX(), getY() + 8);
		if (m_goods == 2) getWorld()->addShroom(getX(), getY() + 8);
		if (m_goods == 3) getWorld()->addFlower(getX(), getY() + 8);
	}
}

Pipe::Pipe(int startX, int startY, StudentWorld* world)
	:Structure(7, startX, startY, world)
{}

Projectile::Projectile(int ImageID, int startX, int startY, int dir, StudentWorld* world)
	:Actor(ImageID, startX, startY, dir, 1, 1, world)
{
}

// Special doSomething for projects
void Projectile::doSomething()
{

	damageTime(); // Custom method for different types of projectiles
	if (getWorld()->canMove(getX(), getY() - 2)) moveTo(getX(), getY() - 2); // Tries to move down
	double newX = getX() + 2 * (getDirection() / (-90) + 1); 
	if (getWorld()->canMove(newX, getY())) moveTo(newX, getY()); // Attemps to move forward
	else // If can't move forward just dies
	{
		setLiving(false); 
		return;
	}
}

void Projectile::bonk()
{}
GoodProj::GoodProj(int ImageID, int startX, int startY, int dir, StudentWorld* world)
	:Projectile(ImageID, startX, startY, dir, world)
{
}

// Friendly Projectiles try to damage mobs
void GoodProj::damageTime()
{
	if (getWorld()->damageAt(getX(), getY()))
	{
		setLiving(false);
	}
}

Shell::Shell(int startX, int startY, int dir, StudentWorld* world)
	: GoodProj(3, startX, startY, dir, world)
{}

PeachFireball::PeachFireball(int startX, int startY, int dir, StudentWorld* world)
	: GoodProj(13, startX, startY, dir, world)
{}

PiranhaFireball::PiranhaFireball(int startX, int startY, int dir, StudentWorld* world)
	: Projectile(12, startX, startY, dir, world)
{}

// Other Projectiles try to damage peach
void Projectile::damageTime()
{
	if (getWorld()->isPeachAt(getX(), getY()))
	{
		getWorld()->damagePeach();
		setLiving(false);
	}
}


LevelEnder::LevelEnder(int ImageID, int startX, int startY, StudentWorld* world)
	:Actor(ImageID, startX, startY, 0, 1, 1, world)
{
}

// Level Ender's special doSomething method
void LevelEnder::doSomething()
{
	if (!isAlive()) return; // if not alive, do nothing
	if(getWorld()->isPeachAt(getX(), getY())) // Checks if peach is at the level ender
	{
		getWorld()->increaseScore(1000); // Awards score
		setLiving(false); // Kills self
		changeHealth(-100); 
		whatHappensNext(); // Custom method for different types of LevelEnders
	}
}

void LevelEnder::bonk()
{}

Flag::Flag(int startX, int startY, StudentWorld* world)
	:LevelEnder(11, startX, startY, world)
{}

// Flags progress to the next level
void Flag::whatHappensNext()
{
	getWorld()->levelDone();
}

Mario::Mario(int startX, int startY, StudentWorld* world)
	: LevelEnder(5, startX, startY, world)
{}

// Mario ends the game
void Mario::whatHappensNext()
{
	getWorld()->gameDone();
}

Mob::Mob(int ImageID, int startX, int startY, StudentWorld* world)
	:Actor(ImageID, startX, startY, 180 * (std::rand() % 2), 1, 0, world)
{}

// Mobs are set to able to be damaged
bool Mob::damageable()
{
	return true;
}

// Mob's special doSomething method
void Mob::doSomething()
{
	if (!isAlive()) return; // If dead do nothing
	if (getWorld()->isPeachAt(getX(), getY())) // Tries to bonk peach
	{
		getWorld()->bonkPeach();
		return;
	}
	double newX = getX() + (getDirection() / (-90) + 1);
	if (!getWorld()->canMove(newX, getY())) changeDirection(); // If at a wall turn the other way
	else if (getDirection() == 180) // Otherwise, check if at an edge; if so, turn the other way
	{
		if (getWorld()->canMovePixel(getX() - 1, getY() - 1))
		{
			changeDirection();
		}
	}
	else if (getDirection() == 0)
	{
		if (getWorld()->canMovePixel(getX() + SPRITE_WIDTH, getY() - 1))
		{
			changeDirection();
		}
	}
	newX = getX() + (getDirection() / (-90) + 1);
	if (getWorld()->canMove(newX, getY())) moveTo(newX, getY()); // Tries to move forward
}

// Special Mob bonk method
void Mob::bonk()
{
	if (getWorld()->doesPeachStar()) // Only works if peach bonked with star power
	{
		getWorld()->playSound(SOUND_PLAYER_KICK); // Plays sound for bonking
		getWorld()->increaseScore(100); // Award player score
		setLiving(false); // The Mob dies
		forKoopa(); // Spawns shells for koopa only
	}
}

// Mob's special damaged metthod
void Mob::damaged()
{
	getWorld()->increaseScore(100); // Awards player's score
	setLiving(false); // The Mob dies
	forKoopa(); // Spawns shells for koopa only
}

Goomba::Goomba(int startX, int startY, StudentWorld* world)
	:Mob(2, startX, startY, world)
{}

Piranha::Piranha(int startX, int startY, StudentWorld* world)
	: Mob(4, startX, startY, world), m_delay(0)
{}

// Piranha is different from other mobs
void Piranha::doSomething()
{
	if (!isAlive()) return; // If dead do nothing
	increaseAnimationNumber(); // Animates the piranha
	if (getWorld()->isPeachAt(getX(), getY())) // Tries to bonk peach
	{
		getWorld()->bonkPeach();
		return;
	}
	if (!getWorld()->peachIsInSight(getY())) return; // Checks to see if peach is in range
	if (getWorld()->peachIsLeft(getX())) setDirection(180); // Turns to peach's direction
	else setDirection(0);
	if (m_delay) // Checks for firing delay
	{
		m_delay--;
		return;
	}
	if (getWorld()->peachIsClose(getX())) // Shoots fireball if peach is close enough
	{
		getWorld()->addPiranhaFireball(getX(), getY(), getDirection());
		getWorld()->playSound(SOUND_PIRANHA_FIRE);
		m_delay = 40;
	}

	
}

Koopa::Koopa(int startX, int startY, StudentWorld* world)
	: Mob(1, startX, startY, world)
{}

// Koopas spawn shells
void Koopa::forKoopa()
{
	getWorld()->addShell(getX(), getY(), getDirection());
}

Goodie::Goodie(int ImageID, int startX, int startY, StudentWorld* world)
	: Actor(ImageID, startX, startY, 0, 1, 1, world)
{}

// Special doSomething method for Goodie
void Goodie::doSomething()
{
	if (getWorld()->isPeachAt(getX(), getY())) // Gives goodie to peach if she is collided with
	{
		upgrade(); // Special upgrade for each item
		setLiving(false); // Deactivates item
		changeHealth(-100);
		getWorld()->playSound(SOUND_PLAYER_POWERUP); // Plays Powerup sound
		return;
	}
	else // Otherwise the item moves
	{
		if (getWorld()->canMove(getX(), getY() - 2)) moveTo(getX(), getY() - 2); // Attempts to move down
		double newX = getX() + 2 * (getDirection() / (-90) + 1); 
		if (getWorld()->canMove(newX, getY())) moveTo(newX, getY()); // Tries to move forward, other wise change direction
		else
		{
			changeDirection();
		}
	}
}	

void Goodie::bonk()
{}

Star::Star(int startX, int startY, StudentWorld* world)
	: Goodie(8, startX, startY, world)
{}

// Gives star power and increases score by 100
void Star::upgrade()
{
	getWorld()->increaseScore(100);
	getWorld()->giveStar();

}

Flower::Flower(int startX, int startY, StudentWorld* world)
	: Goodie(9, startX, startY, world)
{}

// Gives flower power and increases score by 50
void Flower::upgrade()
{
	getWorld()->increaseScore(50);
	getWorld()->giveFlower();
	getWorld()->setPeachHealth(200);
}

Mushroom::Mushroom(int startX, int startY, StudentWorld* world)
	: Goodie(10, startX, startY, world)
{}

// Gives mushroom power and increases score by 75
void Mushroom::upgrade()
{
	getWorld()->increaseScore(75);
	getWorld()->giveJump();
	getWorld()->setPeachHealth(200);
}