#include "Grass.h"

Grass::Grass(World* world) {
	increaseStrength(0);
	setInitiative(0);
	sprite = 'g';
	this->world = world;
	spawn();
}

Grass::Grass(World* world, Coordinate coord) {
	increaseStrength(0);
	setInitiative(0);
	sprite = 'g';
	this->world = world;
	world->board.insert({ coord , this });
}

Organism* Grass::createChild(Coordinate coord) {
	return new Grass(world, coord);
}
