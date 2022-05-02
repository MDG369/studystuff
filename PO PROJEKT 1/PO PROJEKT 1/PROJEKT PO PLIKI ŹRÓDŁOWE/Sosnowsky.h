#pragma once
#include "Nightshade.h"
#include "Animal.h"
class Sosnowsky :
    public Nightshade
{
public:
    Sosnowsky(World* world);
    Sosnowsky(World* world, Coordinate coord);
private: 
    void action(Coordinate coord) override;
    Organism* createChild(Coordinate coord) override;
    void draw() override;
    void destroySides(Coordinate coord);
};

