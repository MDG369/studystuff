#pragma once
#include "Plant.h"
#define STRENGTH_INCREASE 3

class Guarana :
    public Plant
{
public:
    Guarana(World* world);
    Guarana(World* world, Coordinate coord);
private:
    bool hasIncreasedStrength(Coordinate coord) override;
    Organism* createChild(Coordinate coord);
    void draw() override;
};

