#include "Fox.h"

Fox::Fox(World* world) {
    increaseStrength(3);
    setInitiative(7);
    sprite = 'f';
    this->world = world;
    spawn();
}

Fox::Fox(World* world, Coordinate coord) {
    increaseStrength(3);
    setInitiative(7);
    sprite = 'f';
    this->world = world;
    world->board.insert({ coord, this });
}

Coordinate Fox::move(Coordinate coord, int speed) {
    Coordinate newCoord = coord;
    int dir = (rand() % 4);
    bool cycled = false;
    while (true) {
        if (dir == N) {
            newCoord.y -= speed;
            if (newCoord.y < 0) {
                newCoord.y += speed * 2;
            }
        }
        else if (dir == E) {
            newCoord.x += speed;
            if (newCoord.x >= world->getColumns()) {
                newCoord.x -= speed * 2;
            }
        }
        else if (dir == S) {
            newCoord.y += speed;
            if (newCoord.y >= world->getRows()) {
                newCoord.y -= speed * 2;
            }
        }
        else if (dir == W) {
            newCoord.x -= speed;
            if (newCoord.x < 0) {
                newCoord.x += speed * 2;
            }
        }
        if (!checkIfStronger(newCoord)) {
            return newCoord;
        }
        else {
            newCoord = coord;
            
            if (dir < W) {
                dir++;
            }
            else { 
                dir = N;
                if (cycled == true) {
                    break;
                }
                bool cycled = true;
            }
        }
    }
    if (newCoord == coord) {
        cout << " lis siê nie rusza!";
    }
    return coord;
}

bool Fox::checkIfStronger(Coordinate coord) {
    if (world->board.count(coord) && !dynamic_cast<Fox*>(world->board[coord])) { // jeœli s¹siednie zwierze istnieje i nie jest lisem to nale¿y sprawdzic czy jest silniejsze
        if (world->board[coord]->getStrength() > this->getStrength()) {
            cout << "Lis wybiera inne pole" << endl;
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

Organism* Fox::createChild(Coordinate coord) {
    return new Fox(world, coord);
}

void Fox::draw() {
    cout << ORANGE << getSprite() << RESET;
}