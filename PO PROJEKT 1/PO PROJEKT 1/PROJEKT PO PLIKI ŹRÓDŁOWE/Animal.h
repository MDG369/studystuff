#pragma once
#include "Organism.h"
class Animal :
    public Organism
{
public:
protected:
    virtual void action(Coordinate coord);
    virtual void collision(Coordinate coord, Coordinate newCoord);
    virtual void fight(Coordinate coord, Coordinate newCoord);
    virtual Coordinate move(Coordinate coord, int speed);
};

