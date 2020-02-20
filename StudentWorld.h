#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"

#include <string>
#include <vector> 
#include <cmath>
#include <stdlib.h>  

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp




class Actor;
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    virtual int init(); //p16
    virtual int move(); //p17
    virtual void cleanUp(); //p 21

    
    int getActorsCount() const;
    std::vector<Actor*> getActors() const;
    void fireSpray();
private:
    Actor * m_player;
    std::vector<Actor*> m_Actors;
    int m_nBacteria;
};


//USE STRINGSTREAMS 
#endif // STUDENTWORLD_H_
