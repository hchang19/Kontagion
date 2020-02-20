#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>
using namespace std;

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
	delete m_player;
	vector<Actor*>::iterator it;
	for (it = m_Actors.begin(); it != m_Actors.end();) {
		it = m_Actors.erase(it);
	}
}

int StudentWorld::init()
{
	const int CURRENT_LEVEL = getLevel();
	const int MAX_DISTANCE = 120;
	m_player = new Socrates(this);
	m_nBacteria = 0;

	//calculate amount of pit, food and dirt
	int dirt_count = max(180 - 20 * CURRENT_LEVEL, 20);
	int food_count = min(5 * CURRENT_LEVEL, 25);
	double x_pos, y_pos;
	//LOAD PITS - check no pits collide
	//LOAD FOOD - check no food collide

	for (int i = 0; i < food_count;) {
		x_pos = randInt(0, VIEW_WIDTH);
		y_pos = randInt(0, VIEW_HEIGHT);
		if (calculateDistance(x_pos, y_pos, VIEW_WIDTH / 2, VIEW_HEIGHT / 2) <= MAX_DISTANCE) {
			for (int i = 0; i < m_Actors.size(); i++) {
					if (calculateDistance(x_pos, y_pos, m_Actors[i]->getX(), m_Actors[i]->getY() <= SPRITE_WIDTH)) {
						continue;
					}
			}
			Actor* temp = new Food(this, x_pos, y_pos);
			m_Actors.push_back(temp);
			i++;
		}
	}

	//LOAD DIRT
	for (int i = 0; i < dirt_count;) {
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
			Actor* temp = new Dirt(this, x_pos, y_pos);
			m_Actors.push_back(temp);
			i++;
		}
	}


	/////TESTING NEW CLASSES
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

	m_player->doSomething();

	for (int i = 0; i < m_Actors.size(); i++) {
		m_Actors[i]->doSomething();
		if (!m_player->isAlive()) {
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
	}

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



	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete m_player;
	vector<Actor*>::iterator it;
	for (it = m_Actors.begin(); it != m_Actors.end();) {
		it = m_Actors.erase(it);
	}
}

int StudentWorld::getActorsCount() const {

	return m_Actors.size();
}

vector<Actor*> StudentWorld::getActors() const{
	return m_Actors;
}