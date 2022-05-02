#include "Nightshade.h"

Nightshade::Nightshade(World* world) {
	increaseStrength(0);
	setInitiative(0);
	sprite = 'n';
	this->world = world;
	spawn();
}

Nightshade::Nightshade(World* world, Coordinate coord) {
	increaseStrength(0);
	setInitiative(0);
	sprite = 'n';
	this->world = world;
	world->board.insert({ coord, this });
}


Organism* Nightshade::createChild(Coordinate coord) {
	return new Nightshade(world, coord);
}

bool Nightshade::poisonous(Coordinate coord, Coordinate newCoord) {
	cout << world->board[coord]->getSprite() << " zjada trujaca rosline i umiera :(" << endl;
	world->board.erase(coord);
	world->board.erase(newCoord);
	return true;
}

void Nightshade::draw() {
	cout << GREEN_BCKG << BLACK << getSprite() << RESET;
}