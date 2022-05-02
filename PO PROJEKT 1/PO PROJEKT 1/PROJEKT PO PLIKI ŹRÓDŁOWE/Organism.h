#pragma once
#include "Directions.h"
#include "Coordinate.h"
#include <iostream>

class World;

class Organism
{
public: 
	virtual void action(Coordinate coord) = 0;
	virtual void draw();
	virtual void endTurn();
	int getInitiative();
	int getStrength();
	int getAge();
	virtual int getSuperSpeedTimer();
	virtual char getSprite();
	void setInitiative(int initiative);
	void increaseStrength(int strength);
	void setAge(int age);
	Coordinate findCoord();
	virtual bool ranAway(Coordinate coord, Coordinate newCoord);
	virtual bool hasDeflectedAttack(Coordinate coord, Coordinate newCoord);
	virtual bool hasIncreasedStrength(Coordinate coord);
	virtual bool poisonous(Coordinate coord, Coordinate newCoord);
protected:
	virtual void spawn();
	Coordinate checkSurroundings(Coordinate coord);
	World* world;
	virtual void multiply(Coordinate coord);
	char sprite;
private:
	virtual Organism* createChild(Coordinate coord) = 0;
	int initiative = 0;
	int strength = 0;
	int age = 0;
};

#include "World.h"
