#pragma once
#include "Animal.h"
#define DEFLECT_STRENGTH 5
#define MOVE_CHANCE 25
class Turtle :
    public Animal
{
public:
    Turtle(World* world);
    Turtle(World* world, Coordinate coord);
private:
    void draw();
    void action(Coordinate coords);
    Organism* createChild(Coordinate coord);
    bool hasDeflectedAttack(Coordinate coords, Coordinate newCoords) override;
};

