#include "Human.h"

Human::Human(World* world, Coordinate coord) {
    increaseStrength(5);
    setInitiative(4);
    sprite = char(2);
    speed = 1;
    this->world = world;
    world->board.insert({ coord, this });
}

Human::Human(World* world) {
    increaseStrength(5);
    setInitiative(4);
    sprite = char(2);
    speed = 1;
    this->world = world;
    spawn();
}

void Human::action(Coordinate coord) {
    cout << "Wcisnij W S A D lub Spacje by aktywowac zdolnosc" << endl;
	Coordinate newCoord = coord;
	int c = 0;
    superSpeed();
    switch ((c = _getch())) {
    case W_KEY:
        newCoord = move(coord, this->speed, N);
        break;
    case D_KEY:
        newCoord = move(coord, this->speed, E);
        break;
    case S_KEY:
        newCoord = move(coord, this->speed, S);
        break;
    case A_KEY:
        newCoord = move(coord, this->speed, W);
        break;
    case SPACE:
        activateSuperSpeed();
        break;
    }
    collision(coord, newCoord);
}

void Human::superSpeed() {
    if (superSpeedTimer > 2) {
        speed = SUPER_SPEED;
    }
    else if (superSpeedTimer > 0) {
        int n = rand() % 2;
        if (n) {
            speed = SUPER_SPEED;
        }
        else speed = 1;
    }
    else if (superSpeedTimer == 0) {
        cout << "Szybkosc antylopy sie skonczyla!" << endl;
    }
    else {
        speed = 1;
    }
    superSpeedTimer--;
    if (superSpeedTimer < -5) {
        superSpeedTimer = -5;
    }
}

void Human::activateSuperSpeed() {
    if (superSpeedTimer < -4) {
        superSpeedTimer = 5;
        cout << " Aktywowano szybkosc antylopy! " << endl;
    }
    else {
        cout << " Jeszcze nie mozna aktywowac zdolnosci specjalnej" << endl;
    }
}

Coordinate Human::move(Coordinate coord, int speed, int dir) {
    Coordinate newCoord = coord;
    if (dir == N) {
        newCoord.y -= speed;
        if (newCoord.y < 0) {
            newCoord.y = 0;
        }
    }
    else if (dir == E) {
        newCoord.x += speed;
        if (newCoord.x >= world->getColumns()) {
            newCoord.x = world->getColumns() - 1;
        }
    }
    else if (dir == S) {
        newCoord.y += speed;
        if (newCoord.y >= world->getRows()) {
            newCoord.y = world->getRows() - 1;
        }
    }
    else if (dir == W) {
        newCoord.x -= speed;
        if (newCoord.x < 0) {
            newCoord.x = 0;
        }
    }
    return newCoord;
}

void Human::draw() {
    cout << CYAN << this->sprite << RESET;
}

Organism* Human::createChild(Coordinate coord) {
    return nullptr;
}

void Human::endTurn() {
    this->age += 1;
    if (findCoord().x == -1) {
    };
}

void Human::setSuperSpeedTimer(int timer) {
    superSpeedTimer = timer;
}

int Human::getSuperSpeedTimer() {
    return superSpeedTimer;
}