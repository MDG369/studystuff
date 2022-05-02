#pragma once
#include "Animal.h"
class Sheep :
    public Animal
{
public:
    Sheep(World* world);
    Sheep(World* world, Coordinate coord);
private:
    Organism* createChild(Coordinate coord);
};

