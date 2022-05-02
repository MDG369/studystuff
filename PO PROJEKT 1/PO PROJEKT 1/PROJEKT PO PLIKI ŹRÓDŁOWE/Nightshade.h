#pragma once
#include "Plant.h"
class Nightshade :
    public Plant
{
public:
    Nightshade(World* world);
    Nightshade(World* world, Coordinate coord);
    Nightshade() = default;
private:
    virtual void draw() override;
    bool poisonous(Coordinate coord, Coordinate newCoord) override;
    virtual Organism* createChild(Coordinate coord);
};

