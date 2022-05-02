#include "Sheep.h"

Sheep::Sheep(World* world) {
	increaseStrength(4);
	setInitiative(4);
	sprite = 'o';
	this->world = world;
	spawn();
}

Sheep::Sheep(World* world, Coordinate coord) {
	increaseStrength(4);
	setInitiative(4);
	sprite = 'o';
	this->world = world;
	world->board.insert({ coord, this });
}

Organism* Sheep::createChild(Coordinate coord) {
	return new Sheep(world, coord);
}