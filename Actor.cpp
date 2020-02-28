#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
using namespace std;
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

//return the current health of the actor
int Actor::getHealth() const {
	return m_health;
}
//set the health of actor to the input
void Actor::updateHealth(int h) {
	m_health = h;
}
//return true if actor is alive
bool Actor::isAlive() const {
	return (m_health > 0);
}
//return the wojrld pointer
StudentWorld* Actor::getWorld() {
	return m_world;
}

//MUST CALL
void Actor::doSomething() {
	return;
}
/////////////////////
//Virtual functions - used to determine the specific behavior that might spam across classes
/////////////////////

//return true if object can be overlaped when spawning
bool Actor::canOverlap() const {
	return false;
}
//return true if object can be hit by the flame or spray
bool Actor::canHit() const {
	return true;
}
//return true if object can stop another sprite to moving onto it
bool Actor::canBlock() const {
	return false;
}


//return true if it is derived from said base class

bool Actor::isEdible() const {
	return false;
}

bool Actor::isBacteria() const {
	return false;
}

bool Actor::isSalmonella() const {
	return false;
}

bool Actor::isEColi() const {
	return false;
}

//return true if it prevents going to the next level
bool Actor::isLevelObjective() const {
	return false;
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
	addSpray = true;
}

void Socrates::updateFlame(int f) {
	m_nFlame = f;
}

int Socrates::getFlame() const {
	return m_nFlame;
}

int Socrates::getSpray() const {
	return m_nSpray;
}

void Socrates::doSomething() {
	
	if (!isAlive()) {
		return;
	}
	//detect key press
	int keyPressed;
	if (getWorld()->getKey(keyPressed)) {
		double newX, newY;

		switch (keyPressed) {
		case KEY_PRESS_LEFT:
			pos_angle -= 5;
			break;
		case KEY_PRESS_RIGHT:
			pos_angle += 5;
			break;
		//key to fire the spray gun
		case KEY_PRESS_SPACE:
			addSpray = false; //moved this limit up to avoid spamming
			if (m_nSpray > 0) {
				getWorld()->fireSpray();
				getWorld()->playSound(SOUND_PLAYER_SPRAY);
				m_nSpray--;
			}
			break;
		//key to fire the flame thrower
		case KEY_PRESS_ENTER:
			if (m_nFlame > 0) {
				getWorld()->fireFlame();
				getWorld()->playSound(SOUND_PLAYER_FIRE);
				m_nFlame--;
			}
			break;
		default:
			return;
		}
		//if there is a new pos_angle, it will move around the circle
		newX = VIEW_RADIUS * cos(pos_angle * 1.0 / 180 * PI) + VIEW_WIDTH / 2;
		newY = VIEW_RADIUS * sin(pos_angle * 1.0 / 180 * PI) + VIEW_HEIGHT / 2;
		moveTo(newX, newY);
		setDirection(pos_angle + 180);
	}
	else {
		//check whether to refill the spray
		if (!addSpray) {
			addSpray = true;
		}
		else if (m_nSpray < 20 && addSpray) {
			m_nSpray++;
		}
	}

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

bool Dirt::canBlock() const {
	return true;
}

void Dirt::doSomething() {
	return;
}

/////////////////////////
/////class Edible - benefits bacteria and is only edible by bacterias. Do not perish
//////////////////////

Edible::Edible(StudentWorld* world, double startX, double startY)
	:Actor(world, IID_FOOD, startX, startY, 90, 1)
{
	return;
}
bool Edible::isEdible() const {
	return true;
}

bool Edible::canHit() const {
	return false;
}


//////////////////////////
///Implementation of Food - a type of edibles
//////////////////////////

Food::Food(StudentWorld* world, double startX, double startY)
	:Edible(world, startX, startY)
{
	return;
}

void Food::doSomething() {
	return;
}

////////////////////////
/////Implementation of Pit - the bacteria spawner
/////////////////////////////
Pit::Pit(StudentWorld* world, double startX, double startY)
	:Actor(world, IID_PIT, startX, startY, 0, 1, 1)
{
	m_nRegSalmon = 5;
	m_nAgrSalmon = 3;
	m_nEColi = 2;
	m_nTotal = 10;
}

bool Pit::canHit() const {
	return false;
}

bool Pit::isLevelObjective() const {
	return true;
}

void Pit::doSomething() {
	//check if there are any bacteria left in pit
	if (m_nTotal <= 0) {
		updateHealth(getHealth() - 1);
		return;
	}

	//determine whether or not to spawn
	int spawnChance = randInt(1, 50);
	if (spawnChance == 1) {
		std::vector<int> m_valids;

		//get the types of remaining bacteria
		if (m_nRegSalmon > 0) {
			m_valids.push_back(SPAWN_CODE_REGSALMON);
		}

		if (m_nAgrSalmon > 0) {
			m_valids.push_back(SPAWN_CODE_AGROSALMON);
		}

		if (m_nEColi > 0) {
			m_valids.push_back(SPAWN_CODE_REGECOLI);
		}

		//randomly pick a spawn type from the available options
		int pickSpawn = randInt(0, m_valids.size() - 1);

		//decrease the count of chosen bacteria
		switch (m_valids[pickSpawn]) {
		case SPAWN_CODE_REGSALMON:
			m_nRegSalmon--;
			break;
		case SPAWN_CODE_AGROSALMON:
			m_nAgrSalmon--;
			break;
		case SPAWN_CODE_REGECOLI:
			m_nEColi--;
			break;
		}
		//spawn specific bacteria by using spawn code which are global constants
		getWorld()->spawnBacteria(getX(), getY(), m_valids[pickSpawn]);
		m_nTotal--;
	}

	return;
}

////////////////////////////////
///Implementation of Ammo //base of things Socrates can Fire
////////////////////////////////

//Derived class with pass in their max travel distance, starting position, damage, and imageID
Ammo::Ammo(StudentWorld* world, double startX, double startY, int dir, double MaxDistance, int ImageID, int damage)
	:Actor(world, ImageID, startX, startY, dir, 1, 1) {
	
	m_initX = startX;
	m_damage = damage;
	m_initY = startY;
	MAX_DISTANCE = MaxDistance;
}

bool Ammo::canHit() const {
	return false;
}

void Ammo::doSomething() {
	if (!isAlive()) {
		return;
	}
	double m_distance = calculateDistance(m_initX, m_initY, getX(), getY());
	//check if it flied for too long
	if (ceil(m_distance) >= MAX_DISTANCE) {
		updateHealth(getHealth() - 1);
		return;
	}

	//calls a StudentWorld function to check if it hits something
	if (getWorld()->ammoHit(getX(), getY(), m_damage)) {
		updateHealth(getHealth() - 1);
		return;
	}

	moveAngle(getDirection(), SPRITE_WIDTH);
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

///////////////////////
//DROP CLASS - base class for goodies for Socrates to pick up
 /////////////////////////
Drop::Drop(StudentWorld* world, double startX, double startY, int ImageID)
	:Actor(world, ImageID, startX, startY, 0, 1, 1)
{
	m_time = 0;
	m_maxTime = max(randInt(0, 300 - (10 * getWorld()->getLevel()) - 1), 50); 
}

void Drop::doSomething() {
	if (!isAlive()) {
		return;
	}
	//check for overlap with player
	Actor* temp_player = getWorld()->getPlayer();
	if (calculateDistance(getX(), getY(), temp_player->getX(), temp_player->getY()) <= SPRITE_WIDTH) {
		doSpecialDrops();
		updateHealth(getHealth() - 1);
		return;
	}

	//see if it has existed for too long
	if (m_time > m_maxTime) {
		updateHealth(getHealth() - 1);
		return;
	}

	m_time++;
}

////////////////
//HealthGoodie////
////////////////

HealthGoodie::HealthGoodie(StudentWorld* world, double startX, double startY) :
	Drop(world, startX, startY, IID_RESTORE_HEALTH_GOODIE)
{
	return;
}
//reset health to 100 and increase score
void HealthGoodie::doSpecialDrops() {
	getWorld()->increaseScore(250);
	getWorld()->playSound(SOUND_GOT_GOODIE);
	getWorld()->getPlayer()->updateHealth(100);
}

//////////////////////
/////Flame Goodie/////////
////////////////////

FlameGoodie::FlameGoodie(StudentWorld* world, double startX, double startY) :
	Drop(world, startX, startY, IID_FLAME_THROWER_GOODIE)
{
	return;
}

//increase flame count by 5 and increase score
void FlameGoodie::doSpecialDrops() {
	Socrates* player = getWorld()->getPlayer();
	getWorld()->increaseScore(300);
	getWorld()->playSound(SOUND_GOT_GOODIE);
	player->updateFlame(player->getFlame() + 5);
}

/////////////////
/////Life Goodie///
 /////////////////

LifeGoodie::LifeGoodie(StudentWorld* world, double startX, double startY) :
	Drop(world, startX, startY, IID_EXTRA_LIFE_GOODIE)
{
	return;
}

//gives the player extra life and increase points
void LifeGoodie::doSpecialDrops() {
	getWorld()->increaseScore(500);
	getWorld()->playSound(SOUND_GOT_GOODIE);
	getWorld()->incLives();
}

////////////////////
/////Fungus class /////
///////////////////////

Fungus::Fungus(StudentWorld* world, double startX, double startY)
	:Drop(world, startX, startY, IID_FUNGUS)
{
	return;
}

//decrease Socrates health, play hurt sound, and reduce points
void Fungus::doSpecialDrops() {
	Socrates* temp_player = getWorld()->getPlayer();
	getWorld()->increaseScore(-50);
	getWorld()->playSound(SOUND_PLAYER_HURT);
	temp_player->updateHealth(temp_player->getHealth() - 20);
}

///////////////////
/////Bacteria class - base class for all bacterias
//////////////////////

Bacteria::Bacteria(StudentWorld* world, double startX, double startY, int imageID, int health, int damage)
	:Actor(world, imageID, startX, startY, 90, 0, health)
{
	m_nFood = 0;
	m_damage = damage;
}

//when bacteria dies, determine if they should become food
Bacteria::~Bacteria() {
	getWorld()->increaseScore(100);
	int foodChance = randInt(0, 1);
	if (foodChance == 0) {
		getWorld()->spawnFood(getX(), getY());
	}
}

bool Bacteria::isBacteria() const {
	return true;
}

//must kill all bacterias to move onto next level
bool Bacteria::isLevelObjective() const {
	return true;
}

void Bacteria::doSomething() {
	if (!isAlive()) {
		return;
	}

	//see if it hits the player. If it does, damage Socrates and skip detecting food or cloning
	Socrates* temp_player = getWorld()->getPlayer();
	if (calculateDistance(getX(), getY(), temp_player->getX(), temp_player->getY()) <= SPRITE_WIDTH) {
		getWorld()->playSound(SOUND_PLAYER_HURT);
		temp_player->updateHealth(temp_player->getHealth() - m_damage);
		doSpecialBacteria();
		return;
	}

	// check whether or not to clone
	if (m_nFood == 3) {
		double xPos, yPos;
		//calculate new x pos
		xPos = getX();
		if (xPos < VIEW_WIDTH / 2) {
			xPos += SPRITE_WIDTH / 2;
		}
		else if (xPos > VIEW_WIDTH / 2) {
			xPos -= SPRITE_WIDTH / 2;
		}
		//calculate new y pos
		yPos = getY();
		if (yPos < VIEW_HEIGHT / 2) {
			yPos += SPRITE_WIDTH / 2;
		}
		else if (yPos > VIEW_HEIGHT / 2) {
			yPos -= SPRITE_WIDTH / 2;
		}
		//calls spawn bacteria with the special spawn code
		createSpecialClone(xPos, yPos);
		m_nFood = 0;
	}

	//check for food

	if (getWorld()->overlapEdibles(getX(), getY())) {
		m_nFood++;
	}
	//movement for each special type of Bacteria
	doSpecialBacteria();
	//wants to continue moving in same direction
}

/////////////////////////////////////
/////Salmonella class - base class for all types of Salmonella
////////////////////////////////

Salmonella::Salmonella(StudentWorld* world, double startX, double startY, int health, int damage)
	:Bacteria(world, startX, startY,IID_SALMONELLA, health, damage)
{
	m_distancePlan = 0;
}

bool Salmonella::isSalmonella() const {
	return true;
}

void Salmonella::doSpecialBacteria() {
	//boolean for special case of Salmonella Regular or Aggresive
	if (doSpecialSalmonella()) {
		return;
	}
	//check if it has a distance plan
	if (m_distancePlan > 0) {
		m_distancePlan--;

		double xPos, yPos;
		getPositionInThisDirection(getDirection(), 3, xPos, yPos);
		//check if it will be blocked or out of bounds or if it will hit a terrain
		if (getWorld()->overlapTerrain(xPos, yPos) || calculateDistance(xPos, yPos, VIEW_WIDTH / 2, VIEW_HEIGHT / 2) >= VIEW_RADIUS) {
			setDirection(randInt(0, 359));
			m_distancePlan = 10;
		}
		else {
			moveAngle(getDirection(), 3);
		}
		return;
	}
	//no distance plan
	else {
		int temp_dir;
		double xPos, yPos;
		if (getWorld()->findFood(getX(), getY(), temp_dir)) {
			getPositionInThisDirection(temp_dir, 3, xPos, yPos);
			//check if the direction toward the nearest food intercept dirt ADDED TO MAKE SURE IT REMAINS IN THE PETRI DISH
			if (!getWorld()->overlapTerrain(xPos, yPos) && calculateDistance(xPos, yPos, VIEW_WIDTH / 2, VIEW_HEIGHT / 2) < VIEW_RADIUS) {
				//chase after nearest food if there is one and doesn't intercept
				setDirection(temp_dir);
				moveAngle(getDirection(), 3);
				return;
			}
		}

		setDirection(randInt(0, 359));
		m_distancePlan = 10;
	}
}

////////////////////////////////
/////Regular Salmonella/////////
////////////////////////////////
RegularSalmonella::RegularSalmonella(StudentWorld* world, double startX, double startY) 
:Salmonella(world, startX, startY, 4, 1)
{
	return;
}

void RegularSalmonella::createSpecialClone(double startX, double startY) {
	getWorld()->spawnBacteria(startX, startY, SPAWN_CODE_REGSALMON);
}

bool RegularSalmonella::doSpecialSalmonella() {
	return false;
}

//////////////////////////////////
/////AggresiveSalmonella///////
/////////////////////////////

AggresiveSalmonella::AggresiveSalmonella(StudentWorld* world, double startX, double startY) 
	:Salmonella(world, startX, startY, 10, 2)
{
	return;
}

void AggresiveSalmonella::createSpecialClone(double startX, double startY) {
	getWorld()->spawnBacteria(startX, startY, SPAWN_CODE_AGROSALMON);
}

bool AggresiveSalmonella::doSpecialSalmonella() {
	//BUG WHEN IT IS RIGHT NEXT TO SOCRATES
	int dirToPlayer;

	//check if it find Socrates within 72 pixels
	if (getWorld()->findSocrates(getX(),getY(),72,dirToPlayer)) {
		double xPos, yPos;
		getPositionInThisDirection(dirToPlayer, 3, xPos, yPos);
		//chase after socrates if it doesn't go out of bounds or hit dirt
		if (!getWorld()->overlapTerrain(xPos, yPos) && calculateDistance(xPos, yPos, VIEW_WIDTH / 2, VIEW_HEIGHT / 2) < VIEW_RADIUS) {
			setDirection(dirToPlayer);
			moveAngle(getDirection(), 3);
		}
		return true;
	}
	return false;
}

/////////////////////////////
//////EColi Class - base class for all Ecoli 
////////////////////////////

EColi::EColi(StudentWorld* world, double startX, double startY, int health, int damage) 
:Bacteria(world, startX, startY, IID_ECOLI, health, damage)
{
	return;
}

bool EColi::isEColi() const {
	return true;
}

void EColi::doSpecialBacteria() {

	int dirToPlayer;
	//check if it finds socrates within 256 pixels
	if (getWorld()->findSocrates(getX(), getY(), 256, dirToPlayer)) {
		for (int i = 0; i < 10; i++) {
			double xPos, yPos;
			//attempt to move toward socrates
			getPositionInThisDirection(dirToPlayer, 2, xPos, yPos);
			if (!getWorld()->overlapTerrain(xPos, yPos) && calculateDistance(xPos, yPos, VIEW_WIDTH / 2, VIEW_HEIGHT / 2) < VIEW_RADIUS) {
				setDirection(dirToPlayer);
				moveAngle(getDirection(), 2);
				break;
			}
			//turn up to 10 times to get to socrates
			dirToPlayer += 10;
			if (dirToPlayer >= 360) {
				dirToPlayer = dirToPlayer % 360;
			}

		}
		return;
	}
}

////////////////////////////
/////REGULAR ECOLI////////
/////////////////////////

RegularEColi::RegularEColi(StudentWorld* world, double startX, double startY) 
	:EColi(world, startX, startY,5, 4)
{
	return;
}

//spawn REGECOLI when the clone function is cloned
void RegularEColi::createSpecialClone(double startX, double startY) {
	getWorld()->spawnBacteria(startX, startY, SPAWN_CODE_REGECOLI);
}
//deosn't do anything special
bool RegularEColi::doSpecialEColi() {
	return false;
}