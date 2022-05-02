#include "Dandelion.h"



Dandelion::Dandelion(World* world) {
	increaseStrength(0);
	setInitiative(0);
	sprite = 'd';
	this->world = world;
	spawn();
}

Dandelion::Dandelion(World* world, Coordinate coord) {
	increaseStrength(0);
	setInitiative(0);
	sprite = 'd';
	this->world = world;
	world->board.insert({ coord, this });
}

Organism* Dandelion::createChild(Coordinate coord) {
	return new Dandelion(world, coord);
}

void Dandelion::action(Coordinate coord) {
	int growRoll = rand() % 100;
	int growTimes = 0;
	if (growRoll <= GROW_CHANCE) {
		while (growTimes < 3) {
			multiply(coord);
			growTimes++;
		}
	}
}

void Dandelion::draw() {
	cout << BRIGHT_YELLOW << getSprite() << RESET;
}
