#include "Plant.h"

void Plant::action(Coordinate coord) {
	int growRoll = rand() % 100;
	if (growRoll <= GROW_CHANCE) {
		multiply(coord);
	}
}

void Plant::draw() {
	cout << BRIGHT_GREEN << this->getSprite() << RESET;
}
