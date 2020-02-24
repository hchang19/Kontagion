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

int Actor::getHealth() const {
	return m_health;
}

void Actor::updateHealth(int h) {
	m_health = h;
}

bool Actor::isAlive() const {
	return (m_health > 0);
}

StudentWorld* Actor::getWorld() {
	return m_world;
}

//Virtual functions
bool Actor::canOverlap() const {
	return false;
}

bool Actor::canHit() const {
	return true;
}

bool Actor::canBlock() const {
	return false;
}
void Actor::doSomething() {
	return;
}

bool Actor::isEdible() const {
	return false;
}

bool Actor::isBacteria() const {
	return false;
}

bool Actor::isSalmonella() const {
	return false;
}
/////////////////////////
/////class Edible//////
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
			addSpray = false; //to avoid spamming
			if (m_nSpray > 0) {
				getWorld()->fireSpray();
				getWorld()->playSound(SOUND_PLAYER_SPRAY);
				m_nSpray--;
			}
			break;
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

		newX = VIEW_RADIUS * cos(pos_angle * 1.0 / 180 * PI) + VIEW_WIDTH / 2;
		newY = VIEW_RADIUS * sin(pos_angle * 1.0 / 180 * PI) + VIEW_HEIGHT / 2;
		moveTo(newX, newY);
		setDirection(pos_angle + 180);
	}
	else {
		if (!addSpray) {
			addSpray = true;
		}
		else if (m_nSpray < 20 && addSpray) {
			m_nSpray++;
		}
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

bool Dirt::canBlock() const {
	return true;
}

void Dirt::doSomething() {
	return;
}

//////////////////////////
///Implementation of Food
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
/////Implementation of Pit
/////////////////////////////
Pit::Pit(StudentWorld* world, double startX, double startY)
	:Actor(world, IID_PIT, startX, startY, 0, 1, 1)
{
	m_nRegSalmon = 5;
	m_nAgroSalmon = 3;
	m_nEColi = 2;
	m_nTotal = 10;

	m_valids.push_back("RegSalmon"); // 0 
	m_valids.push_back("AgroSalmon"); // 1
	m_valids.push_back("EColi"); //2
}

bool Pit::canHit() const {
	return false;
}

void Pit::doSomething() {
	//TODO GOD HELP ME
	if (m_nTotal <= 0) {
		updateHealth(getHealth() - 1);
		return;
	}

	//int spawnChance = randInt(1, 50);
	//if (spawnChance == 1) {
	//	int pickSpawn = randInt(0, m_valids.size() - 1);
	//	///Choose to use a vector to retain how much 
	//	if (m_valids[pickSpawn] == "EColi") {
	//		//Spawn Ecoli
	//		getWorld()->spawnBacteria(getX(), getY(), SPAWN_CODE_ECOLI);
	//		m_nEColi--;
	//		if (m_nEColi == 0) {
	//			m_valids.erase(m_valids.begin() + pickSpawn);
	//		}
	//		return;
	//	}
	//	else if (m_valids[pickSpawn] == "AgroSalmon") {
	//		//Spawn Aggresive Salmonella
	//		getWorld()->spawnBacteria(getX(), getY(), SPAWN_CODE_AGRSALMON);
	//		m_nAgroSalmon--;
	//		if (m_nAgroSalmon == 0) {
	//			m_valids.erase(m_valids.begin() + pickSpawn);
	//		}
	//		return;
	//	}
	//	else if (m_valids[pickSpawn] == "RegSalmon") {
	//		//Spawn Regular Salmonella
	//		getWorld()->spawnBacteria(getX(), getY(), SPAWN_CODE_REGSALMON);
	//		m_nRegSalmon--;
	//		if (m_nRegSalmon == 0) {
	//			m_valids.erase(m_valids.begin() + pickSpawn);
	//		}
	//		return;
	//	}

	//}

	return;
}

////////////////////////////////
///Implementation of Ammo //parent of flame and spray
////////////////////////////////
Ammo::Ammo(StudentWorld* world, double startX, double startY, int dir, double MaxDistance, int ImageID, int damage)
	:Actor(world, ImageID, startX, startY, dir, 1, 1) {
	m_distance = 0;
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
	m_distance = calculateDistance(m_initX, m_initY, getX(), getY());
	//check if it flied for too long
	if (m_distance >= MAX_DISTANCE) {
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
//DROP CLASS - spec pg.33
 /////////////////////////
Drop::Drop(StudentWorld* world, double startX, double startY, int ImageID)
	:Actor(world, ImageID, startX, startY, 0, 1, 1)
{
	m_time = 0;
	m_maxTime = max(randInt(0, 300 - (10 * getWorld()->getLevel()) - 1), 50); //unclear specs, will specify
}

void Drop::doSomething() {
	if (!isAlive()) {
		return;
	}

	Actor* temp_player = getWorld()->getPlayer();
	if (calculateDistance(getX(), getY(), temp_player->getX(), temp_player->getY()) <= SPRITE_WIDTH) {
		doSpecialDrops();
		updateHealth(getHealth() - 1);
		return;
	}

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
void Fungus::doSpecialDrops() {
	Socrates* temp_player = getWorld()->getPlayer();

	getWorld()->increaseScore(-50);
	temp_player->updateHealth(temp_player->getHealth() - 20);
}

///////////////////
/////Bacteria class////////
//////////////////////

Bacteria::Bacteria(StudentWorld* world, double startX, double startY, int imageID, int health, int damage)
	:Actor(world, imageID, startX, startY, 90, 0, health)
{
	m_nFood = 0;
	m_distancePlan = 0;
	m_damage = damage;
}
bool Bacteria::isBacteria() const {
	return true;
}

void Bacteria::doSomething() {
	if (!isAlive()) {
		return;
	}

	doSpecialBacteria();
	Socrates* temp_player = getWorld()->getPlayer();
	if (calculateDistance(getX(), getY(), temp_player->getX(), temp_player->getY()) <= SPRITE_WIDTH) {
		temp_player->updateHealth(temp_player->getHealth() - m_damage);

		return;
	}

	// check whether or not to spawn another food
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
		//call copyfunction
		createSpecialClone(xPos, yPos);
		m_nFood = 0;
	}

	//check for food

	if (getWorld()->overlapFood(getX(), getY())) {
		m_nFood++;
	}
	//wants to continue moving in same direction
}