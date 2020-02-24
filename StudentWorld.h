#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"

#include <string>
#include <vector> 
#include <cmath>
#include <stdlib.h>  
#include <algorithm>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp


const int SPAWN_CODE_REGSALMON = 0;
const int SPAWN_CODE_AGRSALMON = 1;
const int SPAWN_CODE_ECOLI = 2;

const int SPAWN_CODE_DIRT = 3;
const int SPAWN_CODE_FOOD = 4;
const int SPAWN_CODE_PIT = 5;


class Actor;
class Socrates;
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    virtual int init(); //p16
    virtual int move(); //p17
    virtual void cleanUp(); //p 21

    

    Socrates* getPlayer() const;
    void fireSpray();
    void fireFlame();
    void spawnBacteria(double xPos, double yPos, int spawnCode);
    void createTerrain(int count, int spawnCode);


    bool ammoHit(double xPos, double yPos, int damage);
    bool overlapFood(double xPos, double yPos);
private:
    Socrates * m_player;
    std::vector<Actor*> m_Actors;
    int m_nBacteria;
};

//Auxilliary Functions
double calculateDistance(double startX, double startY, double finalX, double finalY);
//USE STRINGSTREAMS 
#endif // STUDENTWORLD_H_
