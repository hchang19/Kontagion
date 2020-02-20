#include "Actor.h"
#include "StudentWorld.h"
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

///////////////////////////////
//Implementation of Actor Class
///////////////////////////////
Actor::Actor(StudentWorld* world, int imageID, double startX, double startY, int dir, int depth, int health) //dir and depth set to 0
	:GraphObject(imageID, startX, startY, dir, depth)
{
	m_world = world;
	m_health = health;
}

Actor::~Actor() {
	return;
}

int Actor::getHealth() const{
	return m_health;
}

void Actor::updateHealth(int h) {
	m_health = h;
}

bool Actor::isAlive() const{
	return (m_health > 0);
}

StudentWorld* Actor::getWorld() {
	return m_world;
}

//Virtual functions
bool Actor::canOverlap() const {
	return false;
}

bool Actor::canHit() const{
	return true;
}

bool Actor::canBlock() const{
	return false;
}
void Actor::doSomething() {
	return;
}

//////////////////////////////////
//Implementation of Socrates Class
//////////////////////////////////
Socrates::Socrates(StudentWorld* world)
	:Actor(world, IID_PLAYER, 0, VIEW_HEIGHT / 2, 0, 0, 100)
{
	pos_angle = 180;
	m_nFlame = 5;
	m_nSpray = 20;
	addSpray = false;
}

void Socrates::updateFlame(int f) {
	m_nFlame = f;
}

int Socrates::getFlame() const {
	return m_nFlame;
}

int Socrates::getSpray() const{
	return m_nSpray;
}

void Socrates::doSomething() {
	//TODO p . 25
	if (!isAlive()) {
		return;
	}
	int keyPressed;
	if (getWorld()->getKey(keyPressed)) {
		const double PI = 4 * atan(1);
		double newX, newY;
		switch (keyPressed) {
		case KEY_PRESS_LEFT:
			pos_angle -= 5;
			break;
		case KEY_PRESS_RIGHT:
			pos_angle += 5;
			break;
		case KEY_PRESS_SPACE:
			if (m_nSpray > 0) {
				getWorld()->fireSpray();
			}
			break;
		case KEY_PRESS_ENTER:
			break;
		default:
			return;
		}

		newX = VIEW_RADIUS * cos(pos_angle * 1.0 / 180 * PI) + VIEW_WIDTH / 2;
		newY = VIEW_RADIUS * sin(pos_angle * 1.0 / 180 * PI) + VIEW_HEIGHT / 2;
		moveTo(newX, newY);
		setDirection(pos_angle + 180);
	}

	//check for sprays
	//TODO
	return;
}

//////////////////////////////
//Implementation of Dirt Class
//////////////////////////////
Dirt::Dirt(StudentWorld* world, double startX, double startY)
	:Actor(world, IID_DIRT, startX, startY, 0, 1)
{
	return;
}

bool Dirt::canOverlap() const {
	return true;
}

bool Dirt::canBlock() const{
	return true;
}

void Dirt::doSomething() {
	return;
}

//////////////////////////
///Implementation of Food
//////////////////////////

Food::Food(StudentWorld* world, double startX, double startY)
	:Actor(world, IID_FOOD, startX, startY, 90, 1)
{
	return;
}

bool Food::canHit() const {
	return false;
}

void Food::doSomething() {
	return;
}

////////////////////////////////
///Implementation of Ammo //parent of flame and spray
////////////////////////////////
Ammo::Ammo(StudentWorld* world, double startX, double startY, int dir, double MaxDistance, int ImageID, int damage)
:Actor(world, ImageID, startX, startY, dir, 1){
	m_distance = 0;
	m_initX = startX;
	m_damage = damage;
	m_initY = startY;
	MAX_DISTANCE = MaxDistance;
}

 bool Ammo::canHit() const {
	 return false;
}
 
 double Ammo::getInitX() const{
	 return m_initX;
 }

 double Ammo::getInitY() const {
	 return m_initY;
 }


 void Ammo::doSomething() {
	 if (!isAlive()) {
		 return;
	 }

	 if (calculateDistance(m_initX, m_initY, getX(), getY()) >= MAX_DISTANCE) {
		 updateHealth(getHealth() - 1);
		 return;
	 }

	 Actor* temp; 
	 for (int i = 0; i < getWorld()->getActorsCount(); i++) {
		 temp = getWorld()->getActors()[i];
		 if (temp->canHit()) {
			 if (calculateDistance(getX(), getY(), temp->getX(), temp->getY()) <= SPRITE_WIDTH)
			 {
				 temp->updateHealth(temp->getHealth() - m_damage);
				 updateHealth(getHealth() - 1);
				 return;
			 }
		 }
	 }

	 moveAngle(getDirection(), SPRITE_RADIUS * 2); 

 }

 ///////////////////////////
 ///Flame class - spec pg . 29
 ///////////////////////////////

 Flame::Flame(StudentWorld* world, double startX, double startY, int dir)
	 :Ammo(world, startX, startY, dir, 32, IID_FLAME, 5)
 {
	 return;
 }
 ////////////////////////////////
//Spray Class -spec pg .30
 //////////////////////////////
 Spray::Spray(StudentWorld* world, double startX, double startY, int dir) 
 
	 :Ammo(world, startX, startY, dir, 112, IID_SPRAY, 2)
 {
	 return;
 }




 ///////////////////////////////
 //Auxilliary 
 ///////////////////////////////
 double calculateDistance(double startX, double startY, double finalX, double finalY) {
	 double distance = sqrt(pow(finalX - startX, 2) + pow(finalY - startY, 2));

	 return distance;
 }