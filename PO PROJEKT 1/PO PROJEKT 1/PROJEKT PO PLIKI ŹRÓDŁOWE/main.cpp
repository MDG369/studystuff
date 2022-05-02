#pragma once
#include <ctime>
#include "World.h"


int main() {
	srand(time(0));
	World world;
	world.gameMenu();
}