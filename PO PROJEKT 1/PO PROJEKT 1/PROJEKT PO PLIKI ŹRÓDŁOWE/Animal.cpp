#include "Animal.h"
void Animal::action(Coordinate coord) {
    Coordinate newCoord = move(coord, 1);
    if (newCoord == coord) {}
    else {
        collision(coord, newCoord);
    }
}

void Animal::collision(Coordinate coord, Coordinate newCoord) {
    if (world->board.count(newCoord)) {
        if (world->board[newCoord]->getSprite() == this->getSprite()) { 
            if (this->getAge() != 0 && world->board[newCoord]->getAge() != 0) { // jeœli wiek zwierz¹t nie jest równy 0
                multiply(newCoord);
            }
        }
        else {
            fight(coord, newCoord);
        }
    }
    else {
        world->board.erase(coord);
        world->board.insert({newCoord, this });
    }
}

void Animal::fight(Coordinate coord, Coordinate newCoord) { // pamiêæ po walce zwalniania jest w world.cpp w funkcji wykonuj¹cej turê
    if (dynamic_cast<Animal*>(world->board[newCoord])) { // gdy na nowym polu znajduje siê zwierze
        if (!world->board[newCoord]->ranAway(coord, newCoord) && !world->board[newCoord]->hasDeflectedAttack(coord, newCoord)) {
            if (world->board[coord]->getStrength() >= world->board[newCoord]->getStrength()) { 
                cout << "Inicjator to " << this->getSprite() << " ";
                cout << endl << "Walka- " << this->getSprite() << " pokonuje " << world->board[newCoord]->getSprite() << endl;
                world->board[newCoord] = world->board[coord];
                world->board.erase(coord);
            }
            else if (world->board[coord]->getStrength() < world->board[newCoord]->getStrength()) {
                cout << "inicjator to " << this->getSprite() << " ";
                cout << endl << "Walka- " << world->board[newCoord]->getSprite() << " pokonuje " << this->getSprite() << endl;
                world->board.erase(coord);
            }
        }
    }
    else { // jeœli zwierze natrafi na roœlinê zjada j¹
        cout << "Inicjator to " << this->getSprite() << " ";
        cout << endl << "Walka- " << this->getSprite() << " zjada " << world->board[newCoord]->getSprite() << endl;
        if (!world->board[newCoord]->poisonous(coord, newCoord)) {
            world->board[newCoord]->hasIncreasedStrength(coord);
            world->board[newCoord] = world->board[coord];   
            world->board.erase(coord);
        }
    }
}

Coordinate Animal::move(Coordinate coord, int speed) {
    Coordinate newCoord = coord;
    int dir = (rand() % 4);
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
