#pragma once
#include "Animal.h"
class Wolf :
    public Animal
{
public:
    Wolf(World* world);
    Wolf(World *world, Coordinate coord);
private:
    void draw() override;
    Organism* createChild(Coordinate coord);
};

