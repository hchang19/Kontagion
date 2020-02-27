#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"

#include <string>
#include <vector> 
#include <cmath>
#include <stdlib.h>  
#include <algorithm>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

//Spawn Codes - passed into generic functions to allocate the specific type
const int SPAWN_CODE_REGSALMON = 0;
const int SPAWN_CODE_AGROSALMON = 1;
const int SPAWN_CODE_REGECOLI = 2;

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
    bool finishedLevel() const;
    //Spawner Function
    //spawn Spray projectile in front of Socrates
    void fireSpray();
    //spawn flame projectiles in circles around Socrates
    void fireFlame();
    //spawn specific type of Bacteria depending on the spawnCode
    void spawnBacteria(double xPos, double yPos, int spawnCode);
    //used to add non-moving classes in the class init
    void createTerrain(int count, int spawnCode);
    //spawns food class at x Y
    void spawnFood(double xPos, double yPos);

    //Detector functions

    //return true if overlap with canHit actor and recrease that amount of damage
    bool ammoHit(double xPos, double yPos, int damage);
    //return true if xPos yPos overlaps with an edible object
    bool overlapEdibles(double xPos, double yPos);
    //returns true if xPos, and yPox overlaps with any of the canBlock Actor
    bool overlapTerrain(double xPos, double yPos);


    //returns true if there is food within 128 radius. set foodX, foodY to x,y position of the food
    bool findFood(double xPos, double yPos, int& dirToFood);  
    //get the direction to scorates
    bool findSocrates(double xPos, double yPos, int bounds, int& dirToSocrates);
    //Specify which bacteria sound to play when it is hit
    void playBacteriaSound(Actor* b);
    
private:
    Socrates * m_player;
    std::vector<Actor*> m_Actors;

};

//Auxilliary Functions
double calculateDistance(double startX, double startY, double finalX, double finalY);
//USE STRINGSTREAMS 
#endif // STUDENTWORLD_H_
