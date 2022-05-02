#include "Sosnowsky.h"

Sosnowsky::Sosnowsky(World* world) {
	increaseStrength(10);
	setInitiative(0);
	sprite = 's';
	this->world = world;
	spawn();
}

Sosnowsky::Sosnowsky(World* world, Coordinate coord) {
	increaseStrength(10);
	setInitiative(0);
	sprite = 's';
	this->world = world;
	world->board.insert({ coord, this });
}

void Sosnowsky::action(Coordinate coord) {
	destroySides(coord);
	int growRoll = rand() % 100;
	if (growRoll <= GROW_CHANCE) {
		multiply(coord);
	}
}

void Sosnowsky::destroySides(Coordinate coord) {
	Coordinate newCoord = coord;
	newCoord.x += 1;
	if (world->board.count(newCoord) && dynamic_cast<Animal*>(world->board[newCoord])) {
		world->board.erase(newCoord);
	}
	newCoord.x -= 2;
	if (world->board.count(newCoord) && dynamic_cast<Animal*>(world->board[newCoord])) {
		world->board.erase(newCoord);
	}
	newCoord.x += 1;
	newCoord.y -= 1;
	if (world->board.count(newCoord) && dynamic_cast<Animal*>(world->board[newCoord])) {
		world->board.erase(newCoord);
	}
	newCoord.y += 2;
	if (world->board.count(newCoord) && dynamic_cast<Animal*>(world->board[newCoord])) {
		world->board.erase(newCoord);
	}
	newCoord.y -= 1;
}

Organism* Sosnowsky::createChild(Coordinate coord) {
	return new Sosnowsky(world, coord);
}

void Sosnowsky::draw() {
	cout << GREEN_BCKG << getSprite() << RESET;
}