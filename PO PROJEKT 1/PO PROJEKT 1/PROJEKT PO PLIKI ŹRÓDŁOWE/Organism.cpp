#include "Organism.h"

void Organism::draw() {
	cout << this->sprite;
}

void Organism::spawn() {
	Coordinate coords;
    bool spawned = false;
    bool cycled = false;
    if (world->board.size() < world->getColumns() * world->getRows()) {
        coords.y = rand() % world->getRows();
        coords.x = rand() % world->getColumns();
        while (!spawned) { // jeœli wylosowane pole jest zajête wybierane jest nastêpne wolne pole
            if (!world->board.count(coords)) {
                world->board.insert({ coords, this });
                spawned = true;
            }
            else if (coords.x != world->getColumns()) {
                coords.x += 1;
            }
            else if (coords.y != world->getRows()) {
                coords.y += 1;
                coords.x = 0;
            }
            else {
                coords.x = 0;
                coords.y = 0;
            }
        }
    }
}

bool Organism::ranAway(Coordinate coord, Coordinate newCoord) {
	return false;
}

bool Organism::hasDeflectedAttack(Coordinate coord, Coordinate newCoord) {
    return false;
}

bool Organism::hasIncreasedStrength(Coordinate coord) {
    return false;
}

bool Organism::poisonous(Coordinate coord, Coordinate newCoord) {
    return false;
}


Coordinate Organism::checkSurroundings(Coordinate coord) { // w przypadku gdy nie ma wolnego pola, funkcja zwraca niezmieniony koordynat
	Coordinate newCoord = coord;
    bool nClear = true, eClear = true, wClear = true, sClear = true;
    while (nClear || eClear || wClear || sClear) {
        int dir = rand() % 4;
        if (dir == N && nClear == true) {
            newCoord.y -= 1;
            if (newCoord.y < 0) {
                newCoord.y += 1;
                nClear = false;
            }
            else if (!world->board.count(newCoord)) {
                return newCoord;
            }
            else {
                newCoord.y += 1;
                nClear = false;
                dir++;
            }
        }
        if (dir == E && eClear == true) {
            newCoord.x += 1;
            if (newCoord.x >= world->getColumns() || world->board.count(newCoord)) {
                newCoord.x -= 1;
                eClear = false;
            }
            else if (!world->board.count(newCoord)) {
                return newCoord;
            }
            else {
                newCoord.x -= 1;
                eClear = false;
                dir++;
            }

        }
        if (dir == S && sClear == true) {
            newCoord.y += 1;
            if (newCoord.y >= world->getRows() || world->board.count(newCoord)) {
                newCoord.y -= 1;
                sClear = false;
                dir++;
            }
            else if (!world->board.count(newCoord)) {
                return newCoord;
            }
        }
        if (dir == W && wClear == true) {
            newCoord.x -= 1;
            if (newCoord.x < 0 || world->board.count(newCoord)) {
                newCoord.x += 1;
                wClear = false;
            }
            else if (!world->board.count(newCoord)) {
                return newCoord;
            }
        }
    }
    return coord;
}

void Organism::endTurn() {
    age += 1;
}

Coordinate Organism::findCoord() {
    for (auto it = world->board.cbegin(), next_it = it; it != world->board.cend(); it = next_it) {
        if (this == it->second) {
            return it->first;
        }
        ++next_it;
    }
    Coordinate failedCoord;
    failedCoord.x = -1, failedCoord.y = -1;
    return failedCoord;
}

void Organism::multiply(Coordinate coord) {
    Coordinate childCoord = checkSurroundings(coord); // metoda checkSurroundings w przypadku braku wolnego pola w s¹siedztwie zwraca niezmienione pole
    if (childCoord == coord) {

    }
    else {
        Organism* child = createChild(childCoord);
    }
}

int Organism::getInitiative() {
    return this->initiative;
}

int Organism::getStrength() {
    return this->strength;
}

int Organism::getAge() {
    return this->age;
}

int Organism::getSuperSpeedTimer() {
    return NULL;
}

void Organism::setInitiative(int initiative) {
    this->initiative = initiative;
}

void Organism::increaseStrength(int strength) {
    this->strength += strength;
}

char Organism::getSprite() {
    return sprite;
}

void Organism::setAge(int age) {
    this->age = age;
}