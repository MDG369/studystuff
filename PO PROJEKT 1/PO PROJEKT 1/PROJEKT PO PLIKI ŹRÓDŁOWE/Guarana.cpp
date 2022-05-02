#include "Guarana.h"

Guarana::Guarana(World* world) {
	increaseStrength(0);
	setInitiative(0);
	sprite = 'u';
	this->world = world;
	spawn();
}

Guarana::Guarana(World* world, Coordinate coord) {
	increaseStrength(0);
	setInitiative(0);
	sprite = 'u';
	this->world = world;
	world->board.insert({ coord, this });
}

Organism* Guarana::createChild(Coordinate coord) {
	return new Guarana(world, coord);
}

bool Guarana::hasIncreasedStrength(Coordinate coord) {
	world->board[coord]->increaseStrength(STRENGTH_INCREASE);
	cout << world->board[coord]->getSprite() << " zwieksza swoja sile o " << STRENGTH_INCREASE << endl;
	return true;
}

void Guarana::draw() {
	cout << GREEN_BCKG << RED << getSprite() << RESET;
}
