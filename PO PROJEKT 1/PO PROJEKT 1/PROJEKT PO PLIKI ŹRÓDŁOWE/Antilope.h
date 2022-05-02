#pragma once
#include "Animal.h"
#define RUN_CHANCE 50
class Antilope :
    public Animal
{
public:
    Antilope(World* world);
    Antilope(World* world, Coordinate coord);
private:
    void action(Coordinate coord) override;
    void draw() override;
    bool ranAway(Coordinate coord, Coordinate newCoord) override;
    Organism* createChild(Coordinate coord) override;
};

