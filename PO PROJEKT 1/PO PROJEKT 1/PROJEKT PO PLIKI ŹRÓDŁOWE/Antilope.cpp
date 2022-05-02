#include "Antilope.h"

Antilope::Antilope(World* world) {
    increaseStrength(4);
    setInitiative(4);
	sprite = 'a';
	this->world = world;
    spawn();
}

Antilope::Antilope(World* world, Coordinate coord) {
    increaseStrength(4);
    setInitiative(4);
    sprite = 'a';
    this->world = world;
    world->board.insert({ coord, this });
}


void Antilope::action(Coordinate coord) {
    Coordinate newCoord;

    newCoord = move(coord, 2);
    if (newCoord == coord) {}
    else {
        collision(coord, newCoord);
    }
}

bool Antilope::ranAway(Coordinate coord, Coordinate newCoord) {
    Coordinate newCoordtmp = newCoord;
    Coordinate newCoord2 = newCoord;
    bool free = false;
    int n = rand() % 100;
    if (n < RUN_CHANCE) {
        Coordinate newCoord2 = checkSurroundings(newCoord);
        if (newCoord2 == newCoord) {
            cout << "antylopa nie ma gdzie uciec";
            return false;
        }
        else {
            cout << endl << "antylopa uciekla!" << endl;
            Animal::collision(newCoordtmp, newCoord2);
            world->board[newCoord] = world->board[coord];
            world->board.erase(coord);
        }
        return true;
    }
        
    else {
        return false;
    }
}

Organism* Antilope::createChild(Coordinate coord) {
    return new Antilope(world, coord);
}

void Antilope::draw() {
    cout << YELLOW << getSprite() << RESET;
}