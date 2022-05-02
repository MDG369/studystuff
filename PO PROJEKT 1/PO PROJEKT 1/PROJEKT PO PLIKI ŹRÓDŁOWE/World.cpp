#include "World.h"
#include "Wolf.h"
#include "Grass.h"
#include "Sheep.h"
#include "Antilope.h"
#include "Turtle.h"
#include "Fox.h"
#include "Dandelion.h"
#include "Guarana.h"
#include "Nightshade.h"
#include "Sosnowsky.h"
#include "Human.h"

void World::drawWorld() {
	cout << "Michal Zielinski" << endl << "nr albumu: 184372" << endl;
	cout << char(201);
	for (int i = 0; i < columns_; i++) {
		cout << char(H_LINE);
	}
	cout << char(187);
	cout << endl;
	for (int i = 0; i < rows_; i++) {
		cout << char(V_LINE);
		for (int j = 0; j < columns_; j++) {
			Coordinate coord;
			coord.x = j;
			coord.y = i;
			if(board.count(coord)) {
				board[coord]->draw();
			}
			else {
				cout << GRAY << "," << RESET;
			}
		}
		cout << char(V_LINE);
		cout << endl;
	}
	cout << char(200);
	for (int i = 0; i < columns_; i++) {
		cout << char(H_LINE);
	}
	cout << char(188);
	cout << endl;
}

void World::setDimensions(int rows, int columns) {
	rows_ = rows;
	columns_ = columns;
}

void World::doTurn() {
	drawWorld();
	vector<Organism*> vec;
	for (auto it = board.cbegin(), next_it = it; it != board.cend(); it = next_it) { // zapełnianie wektora organizmami w celu ich późniejszego posortowania 
		vec.push_back(it->second );
		++next_it;
	}
	sort(vec.begin(), vec.end(), [](Organism* a, Organism* b) { // sortowanie wektora z organizmami najpierw po inicjatywie a potem po wieku
		if (a->getInitiative() != b->getInitiative()) return a->getInitiative() < b->getInitiative();
		else return a->getAge() < b->getAge(); });

	for (int i = vec.size()-1; i >=0 ; i--) { // iterowanie po wektorze i wykonywanie akcji organizmów
		if (board.count(vec[i]->findCoord())) {
			Coordinate coord = vec[i]->findCoord();
			if (coord.x != -1) {
				vec[i]->action(coord);
			}
		}
		else {
			delete vec[i]; // zwalnianie pamięci po zniszczonych organizmach
			vec.erase(vec.begin() + i);
		}
	}
	for (int i = vec.size() - 1; i >= 0; i--) { // zwalnianie pamięci po zniszczonych organizmach
		if (!board.count(vec[i]->findCoord())) {
			delete vec[i];
			vec.erase(vec.begin() + i);
		}
	}
	for (auto it = board.cbegin(), next_it = it; it != board.cend(); it = next_it) { 
		it->second->endTurn(); // zwiększanie wieku organizmów oraz licznik umiejętności w przypadku człowieka
		++next_it;
	}
}

World::World() {
	rows_ = 20;
	columns_ = 20;
}

World::World(int rows, int columns) {
	rows_ = rows;
	columns_ = columns;

}

World::~World() {
}

int World::getColumns() {
	return columns_;
}

int World::getRows() {
	return rows_;
}

void World::clearBoard() {
	for (auto it = board.cbegin(), next_it = it; it != board.cend(); it = next_it) { // zwalnianie pamięci na koniec gry
		Organism* del = it->second;
		delete del;
		++next_it;
	}
	board.clear();
}

void World::initializeBoard() {
	Sheep* s1 = new Sheep(this);
	Sheep* s2 = new Sheep(this);
	Sheep* s3 = new Sheep(this);
	Wolf* w1 = new Wolf(this);
	Wolf* w2 = new Wolf(this);
	Wolf* w3 = new Wolf(this);
	Wolf* w4 = new Wolf(this);
	Grass* g1 = new Grass(this);
	Grass* g2 = new Grass(this);
	Dandelion* d1 = new Dandelion(this);
	Dandelion* d2 = new Dandelion(this);
	Antilope* a1 = new Antilope(this);
	Antilope* a2 = new Antilope(this);
	Antilope* a3 = new Antilope(this);
	Antilope* a4 = new Antilope(this);
	Antilope* a5 = new Antilope(this);
	Turtle* t1 = new Turtle(this);
	Turtle* t2 = new Turtle(this);
	Turtle* t3 = new Turtle(this);
	Guarana* u1 = new Guarana(this);
	Guarana* u2 = new Guarana(this);
	Nightshade* n1 = new Nightshade(this);
	Nightshade* n2 = new Nightshade(this);
	Fox* f1 = new Fox(this);
	Fox* f2 = new Fox(this);
	Fox* f3 = new Fox(this);
	Fox* f4 = new Fox(this);
	Sosnowsky* b1 = new Sosnowsky(this);
	Sosnowsky* b2 = new Sosnowsky(this);
	Human* h1 = new Human(this);
}

void World::loadState() {
	system("CLS");
	bool opened = false;
	while (!opened) {
		cout << "Podaj nazwe pliku: ";
		string name;
		cin >> name;
		fstream file;
		file.open(name, fstream::in);
		if (!file.good()) {
			cout << "Ten plik nie istnieje" << endl;
		}
		else {
			clearBoard();
			int rows, columns;
			file >> rows >> columns;
			rows_ = rows;
			columns_ = columns;
			readInputs(file);
			opened = true;
		}
		file.close();
	}
}

void World::saveState() {
	system("CLS");
	drawWorld();
	string fileName;
	cout << "Podaj nazwe pliku do ktorego chcesz zapisac stan gry" << endl;
	cin >> fileName;
	ofstream file(fileName);
	file << rows_ << " " << columns_ << endl;
	for (auto it = board.cbegin(), next_it = it; it != board.cend(); it = next_it) {
		if (dynamic_cast<Human*>(it->second))
			file << 'h'<< " " << it->second->getAge() << " " << it->second->getStrength() << " " << it->first.x << " " << it->first.y << 
			" " << it->second->getSuperSpeedTimer() << endl;
		else {
			file << it->second->getSprite() << " " << it->second->getAge() << " " << it->second->getStrength() << " " << it->first.x <<
				" " << it->first.y << endl;
		}
		++next_it;
	}
}

void World::readInputs(std::fstream& file)
{
	char sprite;
	int age;
	int strength;
	Coordinate coord;
	while (!file.eof()) {
		file >> sprite >> age >> strength >> coord.x >> coord.y;
		switch (sprite) {
		case ('h'):
		{
			int superSpeedTimer;
			file >> superSpeedTimer;
			Human* h = new Human(this, coord);
			h->setAge(age);
			h->increaseStrength(strength - h->getStrength());
			h->setSuperSpeedTimer(superSpeedTimer);
		}
		break;
		case ('w'):
		{
			Wolf* w = new Wolf(this, coord);
			w->setAge(age);
			w->increaseStrength(strength - w->getStrength());
			break;
		}
		case ('o'):
		{
			Sheep* o = new Sheep(this, coord);
			o->setAge(age);
			o->increaseStrength(strength - o->getStrength());
		}
		break;
		case ('f'):
		{
			Fox* f = new Fox(this, coord);
			f->setAge(age);
			f->increaseStrength(strength - f->getStrength());
		}
		break;
		case ('t'):
		{
			Turtle* t = new Turtle(this, coord);
			t->setAge(age);
			t->increaseStrength(strength - t->getStrength());
		}
		break;
		case ('a'):
		{
			Antilope* a = new Antilope(this, coord);
			a->setAge(age);
			a->increaseStrength(strength - a->getStrength());
		}
		break;
		case ('d'):
		{
			Dandelion* d = new Dandelion(this, coord);
			d->setAge(age);
			d->increaseStrength(strength - d->getStrength());
		}
		break;
		case ('g'):
		{
			Grass* g = new Grass(this, coord);
			g->setAge(age);
			g->increaseStrength(strength - g->getStrength());
		}
		break;
		case ('u'):
		{
			Guarana* u = new Guarana(this, coord);
			u->setAge(age);
			u->increaseStrength(strength - u->getStrength());
		}
		break;
		case ('s'):
		{
			Sosnowsky* s = new Sosnowsky(this, coord);
			s->setAge(age);
			s->increaseStrength(strength - s->getStrength());
		}
		break;
		case ('n'):
		{
			Nightshade* n = new Nightshade(this, coord);
			n->setAge(age);
			n->increaseStrength(strength - n->getStrength());
		}
		break;
		}
	}
}

void World::gameMenu() {
	char option = 'a';
	while (option != char(27)) {
		cout << "MENU GLOWNE" << endl << endl;
		cout << RED << "1" << RESET << "- podstawowa plansza 20x20" << endl;
		cout << RED << "2" << RESET << "- wczytaj stan gry z pliku" << endl;
		cout << "bazowe pliki do testu maja format test(nr).txt np. test1.txt" << endl;
		cout << RED << "3" << RESET << "- utworz plansze podstawowa o podanym rozmiarze (wiersze, kolumny)" << endl;
		cout << "Aby zakonczyc dzialanie programu wcisnij " << RED << "ESC" << RESET << endl;
		option = _getch();
		switch (option) {
		case('1'):
			setDimensions(20, 20);
			initializeBoard();
			break;
		case('2'):
			loadState();
			break;
		case('3'):
			system("CLS");
			int rows, columns;
			cin >> rows >> columns;
			setDimensions(rows, columns);
			initializeBoard();
			break;
		}
		int turnCount = 0;
		while (option != '5' && option != char(27)) {
			doTurn();
			cout << endl << PURPLE << "Wcisnij 4 aby zapisac stan gry do pliku " << endl <<
				"Wcisnij 5 by wrocic do menu" << endl << "Wcisnij dowolny inny przycisk by przejsc do nastepnej tury" << RESET;
			option = _getch();
			if (option == '4') {
				saveState();
			}
			system("CLS");
			turnCount++;
		}
		clearBoard();
	}
}