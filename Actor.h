#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include <cmath>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
//base class for all actors
class Actor :public GraphObject {
public:
	Actor(StudentWorld* world, int imageID, double startX, double startY, int dir = 0, int depth = 0, int health = 1);
	virtual ~Actor();

	bool isAlive() const; //return tr   ue if alive
	StudentWorld* getWorld(); //pointer to the student world

	virtual void doSomething() = 0;
	virtual bool canOverlap() const; //default false
	virtual bool canHit() const; //default true
	virtual bool canBlock()const; //default false
	int getHealth() const;
	void updateHealth(int h);

private:
	StudentWorld* m_world;
	int m_health;
};

//Socrates class - aka the Player - spec p.25

class Socrates :public Actor {
public:
	Socrates(StudentWorld* world);

	virtual void doSomething();
	void updateFlame(int f);

	int getFlame() const;
	int getSpray() const;
private:

	int m_nFlame;
	int m_nSpray;
	double pos_angle;
	bool addSpray;
};

//Dirt class - does nothing each tick - spec p.27
class Dirt : public Actor {
public:
	Dirt(StudentWorld* world, double startX, double startY);
	virtual void doSomething();
	virtual bool canOverlap() const;
	virtual bool canBlock() const;
};
//Food class - can be eaten by Bacteria - spec pg. 28
class Food : public Actor {
public:
	Food(StudentWorld* world, double startxX, double startY);
	virtual void doSomething();
	virtual bool canHit() const;
private:
};


class Ammo : public Actor {
public:
	Ammo(StudentWorld* world, double startX, double startY, int dir, double MaxDistance, int ImageID, int damage);
	virtual void doSomething();
	virtual bool canHit() const;
private:
	double m_distance;
	double MAX_DISTANCE;
	int m_damage;
	double m_initX, m_initY;

};
//Flame class - everything is lit - spec p. 29

class Flame : public Ammo {
public:
	Flame(StudentWorld* world, double startX, double startY, int dir);
};

//Spray class - pew pew = spec pg . 30
class Spray :public Ammo {
public:
	Spray(StudentWorld* world, double startX, double startY, int dir);
};

//Drops -  base class for goodies and fungus - spec pg. 33

class Drop : public Actor {
public:
	Drop(StudentWorld* world, double startX, double startY, int ImageID, int sound, int level);
	virtual void doSomething();
private:
	virtual void doSpecialDrops() = 0;
	int m_time;
	int m_maxTime;


};

//Auxilliary Functions
double calculateDistance(double startX, double startY, double finalX, double finalY) ;
#endif // ACTOR_H_
