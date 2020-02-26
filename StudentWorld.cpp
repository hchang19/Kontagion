#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
using namespace std;

///////////////////////////////





///////////THE DAMAGE FOR EACH BACTERIA IS SET TO 0 FOR TESTING PURPOSE CHANGE BACK BEFORE YOU SUBMIT 
////////////REMEMBER DUMBASS






///////////////////////////
GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
	: GameWorld(assetPath)
{
	m_nBacteria = 0;
	m_player = nullptr;
}

StudentWorld::~StudentWorld() {
	cleanUp();
}

int StudentWorld::init()
{
	const int CURRENT_LEVEL = getLevel();
	m_player = new Socrates(this);
	m_nBacteria = 0;
	
	//calculate amount of pit, food and dirt
	int dirt_count = max(180 - 20 * CURRENT_LEVEL, 20);
	int food_count = min(5 * CURRENT_LEVEL, 25);
	int pit_count = getLevel();


	//LOAD PITS - check no pits collide
	createTerrain(pit_count, SPAWN_CODE_PIT);
	//LOAD FOOD - check no food collide

	//createTerrain(food_count, SPAWN_CODE_FOOD);
	//LOAD DIRT - can overlap dirt

	createTerrain(dirt_count, SPAWN_CODE_DIRT);

	/////TESTING NEW CLASSES
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	const double PI = 4 * atan(1);
	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.


	//each object do Something
	m_player->doSomething();
	for (int i = 0; i < m_Actors.size(); i++) {
		if (m_Actors[i]->isAlive()) {
			m_Actors[i]->doSomething();
		}
	}

	if (!m_player->isAlive()) {
		decLives();
		playSound(SOUND_PLAYER_DIE);
		return GWSTATUS_PLAYER_DIED;
	}


	if (finishedLevel()) {
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}

	//cleanup dead actors
	vector<Actor*>::iterator it;
	for (it = m_Actors.begin(); it != m_Actors.end();) {
		if (!(*it)->isAlive()) {
			delete* it;
			it = m_Actors.erase(it);
		}
		else {
			it++;
		}
	}
	//Add new goodies


	int chanceFungus, chanceGoodie, spawnFungus, spawnGoodie, pos_angle;
	double x_pos,y_pos;
	chanceFungus = max(510 - getLevel() * 10, 200);
	chanceGoodie = max(510 - getLevel() * 10, 250);
	//calculate chance for fungus spawn

	spawnFungus = randInt(0, chanceFungus - 1);
	if (spawnFungus == 0) {
		pos_angle = randInt(0, 360);
		x_pos = VIEW_RADIUS * cos(pos_angle * 1.0 / 180 * PI) + VIEW_WIDTH / 2;
		y_pos = VIEW_RADIUS * sin(pos_angle * 1.0 / 180 * PI) + VIEW_HEIGHT / 2;
		Actor* temp = new Fungus(this, x_pos, y_pos);
		m_Actors.push_back(temp);
	}

	//calculate goodie spawn 
	spawnGoodie = randInt(0, chanceGoodie - 1);
	if (spawnGoodie == 0) {
		int determine = randInt(1, 100);
		Actor* temp;
		pos_angle = randInt(0, 360);
		x_pos = VIEW_RADIUS * cos(pos_angle * 1.0 / 180 * PI) + VIEW_WIDTH / 2;
		y_pos = VIEW_RADIUS * sin(pos_angle * 1.0 / 180 * PI) + VIEW_HEIGHT / 2;
		//spawns healthGoodie
		if (determine <= 60) {
			temp = new HealthGoodie(this, x_pos, y_pos);
		}
		//spawns flameGoodie
		else if (determine <= 90) {
			temp = new FlameGoodie(this, x_pos, y_pos);

		}
		//spawn LifeGoodie
		else {
			temp = new LifeGoodie(this, x_pos, y_pos);
		}
		m_Actors.push_back(temp);
	}

	//Set the status bar text

	//ensure the minimum score is 0 
	if (getScore() < 50) {
		increaseScore(-getScore());
	}

	ostringstream oss;

	oss << "Score: ";
	oss.fill('0');
	oss << setw(6) << getScore() << "  ";
	
	oss.fill(' ');
	oss << "Level: " << setw(2) << getLevel() << "  ";

	oss << "Lives: " << setw(2) << getLives() << "  ";

	oss << "Health: " <<setw(3) << m_player->getHealth() << "  ";

	oss << "Sprays: " << m_player->getSpray() << "  ";

	oss << "Flames: " << setw(2) << m_player->getFlame() << "  ";
	setGameStatText(oss.str());


	

	
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete m_player;
	vector<Actor*>::iterator it;
	for (it = m_Actors.begin(); it != m_Actors.end();) {
		delete* it;
		it = m_Actors.erase(it);
	}
}

Socrates* StudentWorld::getPlayer() const {
	return m_player;
}

bool StudentWorld::finishedLevel() const {
	for (int i = 0; i < m_Actors.size(); i++) {
		if (m_Actors[i]->isLevelObjective()) {
			return false;
		}
	}
	return true;
}

//////////////////
///FOR SPAWNING ///
///////////////////


//Spawns Spray class when Socrates call for it
void StudentWorld::fireSpray() {
	double x_pos, y_pos;
	m_player->getPositionInThisDirection(m_player->getDirection(), SPRITE_WIDTH, x_pos, y_pos);
	Actor* temp = new Spray(this, x_pos, y_pos, m_player->getDirection());
	m_Actors.push_back(temp);
}

//Spawn Flame class when Socrates call for it
void StudentWorld::fireFlame() {
	double x_pos, y_pos;
	int temp_dir = m_player->getDirection();
	for (int i = 0; i < 16; i++) {
		m_player->getPositionInThisDirection(temp_dir, SPRITE_WIDTH, x_pos, y_pos);
		Actor* temp = new Flame(this, x_pos, y_pos, temp_dir);
		m_Actors.push_back(temp);
		temp_dir += 22;
	}
}

//Spawn Bacteria - TODO
void StudentWorld::spawnBacteria(double xPos, double yPos, int spawnCode) {
	Actor* temp;
	
	switch (spawnCode) {
	case SPAWN_CODE_REGSALMON:
		temp = new RegularSalmonella(this, xPos, yPos);
		break;
	case SPAWN_CODE_AGROSALMON:
		temp = new AggresiveSalmonella(this, xPos, yPos);
		break;
	case SPAWN_CODE_REGECOLI:
		temp = new RegularEColi(this, xPos, yPos);
		break;
	default:
		return;
	}
	m_Actors.push_back(temp);
	playSound(SOUND_BACTERIUM_BORN);
	m_nBacteria++;
}

//Spawn Food

void StudentWorld::spawnFood(double xPos, double yPos) {
	Actor* temp;
	temp = new Food(this, xPos, yPos);
	m_Actors.push_back(temp);
}

//Spawn pit, dirt, and food
void StudentWorld::createTerrain(int count, int spawnCode) {
	double x_pos, y_pos;
	const int MAX_DISTANCE = 120;

	for (int i = 0; i < count;) {
		x_pos = randInt(0, VIEW_WIDTH);
		y_pos = randInt(0, VIEW_HEIGHT);
		if (calculateDistance(x_pos, y_pos, VIEW_WIDTH / 2, VIEW_HEIGHT / 2) <= MAX_DISTANCE) {
			for (int i = 0; i < m_Actors.size(); i++) {
				if (!m_Actors[i]->canOverlap()) {
					if (calculateDistance(x_pos, y_pos, m_Actors[i]->getX(), m_Actors[i]->getY() <= SPRITE_WIDTH)) {
						continue;
					}
				}
			}
			Actor* temp;
			switch (spawnCode) {
			case SPAWN_CODE_DIRT:
				temp = new Dirt(this, x_pos, y_pos);
				break;
			case SPAWN_CODE_FOOD:
				temp = new Food(this, x_pos, y_pos);
				break;
			case SPAWN_CODE_PIT:
				temp = new Pit(this, x_pos, y_pos);
				break;
			default:
				return;
			}

			m_Actors.push_back(temp);
			i++;
		}
	}
}

//Spawn Bacteria when the pit determines it
bool StudentWorld:: ammoHit(double xPos, double yPos, int damage) {

	for (int i = 0; i < m_Actors.size(); i++) {
		if(m_Actors[i]->canHit()) {
			if (calculateDistance(xPos, yPos, m_Actors[i]->getX(), m_Actors[i]->getY()) <= SPRITE_WIDTH) {
				m_Actors[i]->updateHealth(m_Actors[i]->getHealth() - damage);
				if (m_Actors[i]->isBacteria()) {
					playBacteriaSound(m_Actors[i]);
				}
				

				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::overlapEdibles(double xPos, double yPos) {
	for (int i = 0; i < m_Actors.size(); i++) {
		if (m_Actors[i]->isEdible()) {
			if (calculateDistance(xPos, yPos, m_Actors[i]->getX(), m_Actors[i]->getY()) <= SPRITE_WIDTH) {
				m_Actors[i]->updateHealth(m_Actors[i]->getHealth() - 1);
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::overlapTerrain(double xPos, double yPos) {
	for (int i = 0; i < m_Actors.size(); i++) {
		if (m_Actors[i]->canBlock()) {
			if (calculateDistance(xPos, yPos, m_Actors[i]->getX(), m_Actors[i]->getY()) <= SPRITE_WIDTH/2) {
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::findFood(double xPos, double yPos, int& dirToFood) {
	Actor* closest_Food = nullptr;
	double min_distance = -1;
	
	for (int i = 0; i < m_Actors.size(); i++) {
		if (m_Actors[i]->isEdible()) {
			double disToFood = calculateDistance(xPos, yPos, m_Actors[i]->getX(), m_Actors[i]->getY());
			if (disToFood > 128) {
				continue;
			}
			else {
				//see if it is the first one to be within 128 
				if (min_distance == -1 || min_distance > disToFood) {
					min_distance = disToFood;
					closest_Food = m_Actors[i];
				}
			}
		}
	}

	if (min_distance == -1 && closest_Food == nullptr) {
		return false;
		dirToFood = -1;
	}

	else {
		dirToFood = atan((closest_Food->getY() - yPos) / (closest_Food->getX() - xPos)) * 180 * PI;
		return true;
	}

}

bool StudentWorld::findSocrates(double xPos, double yPos, int bounds, int& dirToSocrates) {
	double disToPlayer = calculateDistance(xPos, yPos, m_player->getX(), m_player->getY());

	if (disToPlayer <= bounds) {
		dirToSocrates = atan((m_player->getY() - yPos) / (m_player->getX() - xPos)) * 180 * PI;
		return true;
	}
	return false;

}

void StudentWorld::playBacteriaSound(Actor* b) {
	if (b->isSalmonella()) {
		if (b->isAlive()) {
			playSound(SOUND_SALMONELLA_HURT);
		}
		else
		{
			playSound(SOUND_SALMONELLA_DIE);
		}
	}

	if (b->isEColi()) {
		if (b->isAlive()) {
			playSound(SOUND_ECOLI_HURT);
		}
		else
		{
			playSound(SOUND_ECOLI_DIE);
		}
	}
}

///////////////////////////////
 //Auxilliary 
 ///////////////////////////////
double calculateDistance(double startX, double startY, double finalX, double finalY) {
	double distance = sqrt(pow(finalX - startX, 2) + pow(finalY - startY, 2));
	return ceil(distance);
}