#include "Wolf.h"

Wolf::Wolf(World* world) {
	increaseStrength(9);
	setInitiative(5);
	sprite = 'w';
	this->world = world;
	spawn();
}

Wolf::Wolf(World* world, Coordinate coord) {
	increaseStrength(9);
	setInitiative(5);
	sprite = 'w';
	this->world = world;
	world->board.insert({ coord, this });
}

Organism* Wolf::createChild( Coordinate coord){
	return new Wolf(world, coord);
}

void Wolf::draw() {
	cout << GRAY_BCKG << getSprite() << RESET;
}