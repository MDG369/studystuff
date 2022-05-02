#include "Turtle.h"

Turtle::Turtle(World* world) {
    increaseStrength(2);
    setInitiative(1);
    sprite = 't';
    this->world = world;
    spawn();
}

Turtle::Turtle(World* world, Coordinate coord) {
    increaseStrength(2);
    setInitiative(1);
    sprite = 't';
    this->world = world;
    world->board.insert({ coord, this });
}

void Turtle::action(Coordinate coord) {
    int n = rand() % 100;
    if (n < MOVE_CHANCE) {
        Coordinate newCoord = move(coord, 1);
        collision(coord, newCoord);
    }
}

Organism* Turtle::createChild( Coordinate coord) {
    return new Turtle(world, coord);
}

bool Turtle::hasDeflectedAttack(Coordinate coord, Coordinate newCoord) {
    if (world->board[coord]->getStrength() < DEFLECT_STRENGTH) {
        cout << "Zolw odbija atak" << endl;
        return true;
    }
    else {
        return false;
    }
}

void Turtle::draw() {
    cout << BRIGHT_BLUE << getSprite() << RESET;
}