#pragma once
#include "Organism.h"

#define GROW_CHANCE 25
class Plant :
    public Organism
{
private:
    virtual void action(Coordinate coord);
    virtual void draw() override;
};

