#pragma once
#include "Plant.h"
class Grass :
    public Plant
{
public:
    Grass(World* world);
    Grass(World* world, Coordinate coord);
private:
    Organism* createChild(Coordinate coord) override;
};

