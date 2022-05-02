#pragma once
#include <map>
#include <iostream>
#include "Organism.h"
#include <algorithm>
#include <vector>
#include <fstream>

#define GRAY "\033[90m"
#define RESET "\033[0m"
#define GREEN   "\033[32m"
#define BRIGHT_GREEN "\033[92m"
#define YELLOW_BCKG "\033[43m"
#define GREEN_BCKG "\033[42m"
#define CYAN    "\033[36m"
#define PURPLE "\033[35m"
#define RED "\033[31m"
#define BLACK "\033[30m"
#define YELLOW "\033[33m"
#define BRIGHT_YELLOW "\033[93m"
#define ORANGE "\033[91m"
#define BRIGHT_BLUE "\033[96m"
#define GRAY_BCKG "\033[100m"
#define H_LINE 205
#define V_LINE 186
using namespace std;
class World
{
public:
	World();
	~World();
	World(int rows, int columns);
	void drawWorld();
	void setDimensions(int rows, int columns);
	int getRows();
	int getColumns();
	map<Coordinate, Organism*> board;
	void gameMenu();
private:
	void readInputs(std::fstream& file);
	void doTurn();
	void clearBoard();
	void initializeBoard();
	void loadState();
	void saveState();
	int rows_ = 0;
	int columns_ = 0;
};



