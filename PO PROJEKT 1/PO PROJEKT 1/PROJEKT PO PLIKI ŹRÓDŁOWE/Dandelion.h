#pragma once
#include "Plant.h"
class Dandelion :
    public Plant
{
public:
    Dandelion(World* world);
    Dandelion(World* world, Coordinate coord);
private:
    void draw() override;
    Organism* createChild(Coordinate coord);
    void action(Coordinate coords) override;
};

