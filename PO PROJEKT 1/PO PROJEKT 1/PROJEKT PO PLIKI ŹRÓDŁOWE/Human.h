#pragma once
#include "Animal.h"
#include <conio.h>
#define W_KEY 119
#define S_KEY 115
#define A_KEY 97
#define D_KEY 100
#define SPACE 32
#define SUPER_SPEED 2
class Human :
    public Animal
{
public:
    Human(World* world);
    Human(World* world, Coordinate coord);
    void setSuperSpeedTimer(int timer);
    int getSuperSpeedTimer();
protected:
    void draw() override;
private:
    void action(Coordinate coord) override;
    Coordinate move(Coordinate coord, int speed, int dir);
    void superSpeed();
    void activateSuperSpeed();
    Organism* createChild(Coordinate coord);
    int speed = 1;
    int superSpeedTimer = -5;
    int age = 0;
    void endTurn() override;
};

