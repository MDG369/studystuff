#pragma once
#include "Animal.h"
class Fox :
    public Animal
{
public:
    Fox(World* world);
    Fox(World* world, Coordinate coord);
private:
    Coordinate move(Coordinate coord, int speed) override;
    Organism* createChild(Coordinate coord);
    bool checkIfStronger(Coordinate coord);
    void draw();
};

