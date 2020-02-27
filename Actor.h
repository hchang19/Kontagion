#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include <cmath>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
//
const double PI = 4 * atan(1);
//base class for all actors
class Actor :public GraphObject {
public:
	Actor(StudentWorld* world, int imageID, double startX, double startY, int dir = 0, int depth = 0, int health = 1);
	virtual ~Actor();

	bool isAlive() const; //return true if alive
	StudentWorld* getWorld(); //pointer to the student world

	virtual void doSomething() = 0;

	//for purpose of class
	virtual bool canOverlap() const; //default false
	virtual bool canHit() const; //default true
	virtual bool canBlock()const; //default false

	//for Grouping of class
	virtual bool isEdible() const; //default false
	virtual bool isBacteria() const; //default false;
	virtual bool isSalmonella() const; //default false
	virtual bool isEColi() const; 
	virtual bool isLevelObjective() const;
	//Health functions
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

/////////////////////
////Pit class - spawns Bacterias
////////////////////

class Pit :public Actor {
public:
	Pit(StudentWorld* world, double startX, double startY);
	virtual bool canHit() const;
	virtual void doSomething();
	virtual bool isLevelObjective() const;
private:

	int m_nTotal;
	int m_nRegSalmon;
	int m_nAgrSalmon;
	int m_nEColi;
};

//Base class for powerups of Bacteria (currently only contains food)
class Edible : public Actor {
public:
	Edible(StudentWorld* world, double startX, double startY);
	virtual bool isEdible() const;
	virtual bool canHit() const;
};

//Food class - can be eaten by Bacteria - spec pg. 28
class Food : public Edible {
public:
	Food(StudentWorld* world, double startxX, double startY);
	virtual void doSomething();
private:
};

//base class for all projectiles used by Socrates
class Ammo : public Actor {
public:
	Ammo(StudentWorld* world, double startX, double startY, int dir, double MaxDistance, int ImageID, int damage);
	virtual void doSomething();
	virtual bool canHit() const;
private:
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
	Drop(StudentWorld* world, double startX, double startY, int ImageID);
	virtual void doSomething();
private:
	virtual void doSpecialDrops() = 0;
	int m_time;
	int m_maxTime;
};

///////////////////////
//Restore Health////////
//////////////////

class HealthGoodie : public Drop {
public:
	HealthGoodie(StudentWorld* world, double startX, double startY);
private:
	virtual  void doSpecialDrops();
};

///////////////////////
///Restore Flame
//////////////////////
class FlameGoodie : public Drop {
public:
	FlameGoodie(StudentWorld* world, double startX, double startY);
private:
	virtual  void doSpecialDrops();
};

////////////////////////
////Extralife
///////////////////

class LifeGoodie :public Drop {
public:
	LifeGoodie(StudentWorld* world, double startX, double startY);
private:
	virtual void doSpecialDrops();
};

//////////////////////
class Fungus :public Drop {
public:
	Fungus(StudentWorld* world, double startX, double startY);
private:
	virtual void doSpecialDrops();
};


///////////////////////////
/////Bacteria Class - Base class for all Bacterias aka Enemeis of Socrates
///////////////////////////
class Bacteria :public Actor {
public:
	Bacteria(StudentWorld* world, double startX, double startY, int imageID, int health, int damage);
	~Bacteria();
	virtual void doSomething();
	virtual bool isBacteria() const;
	virtual bool isLevelObjective() const;
private:

	
	int m_nFood;
	int m_damage;
	virtual void doSpecialBacteria() = 0;
	virtual void createSpecialClone(double startX, double startY) = 0;
};

//////////////////////////
/////Salmonella class - base class for speicialized Salmonella class
//////////////////////////
class Salmonella :public Bacteria {
public:
	Salmonella(StudentWorld* world, double startX, double startY, int health, int damage);
	virtual bool isSalmonella() const;
private:
	double m_distancePlan;
	virtual void doSpecialBacteria();
	virtual bool doSpecialSalmonella() = 0; //return false to do nothing
};

class RegularSalmonella :public Salmonella {
public:
	RegularSalmonella(StudentWorld* world, double startX, double startY);
private:
	virtual void createSpecialClone(double startX, double startY);
	virtual bool doSpecialSalmonella();

};

class AggresiveSalmonella :public Salmonella {
public:
	AggresiveSalmonella(StudentWorld* world, double startX, double startY);
private:
	virtual void createSpecialClone(double startX, double startY);
	virtual bool doSpecialSalmonella();
};

/////////////////////
/////ECOLI CLASS///////
///////////////////////

class EColi :public Bacteria {
public:
	EColi(StudentWorld* world, double startX, double startY, int health, int damage);
	virtual bool isEColi() const;
private:
	virtual void doSpecialBacteria();
	virtual bool doSpecialEColi() = 0; //return false to do nothing
	
};

class RegularEColi:public EColi {
public:
	RegularEColi(StudentWorld* world, double startX, double startY);
private:
	virtual void createSpecialClone(double startX, double startY);
	virtual bool doSpecialEColi();

};
#endif // ACTOR_H_
