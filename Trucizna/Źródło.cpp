#include <iostream>
using namespace std;
#define DECK_SIZE 500

struct deck_type {
	int* deck_values;
	const char** deck_colors;

	deck_type(int deck_size) {
		deck_values = new int[deck_size];
		deck_colors = new const char* [deck_size];
	}

	~deck_type() {
		delete[] deck_values;
		delete[] deck_colors;
	}
};

struct deck_type* create_deck(int k, int g, int gv, int o, int values[128]) {
	enum kolory {
		green,
		blue,
		red,
		violet,
		yellow,
		white,
		black,
	};
	const char* colors[7] = { "green", "blue", "red", "violet", "yellow","white","black" };

	struct deck_type* deck = new deck_type(k * o + g);


	for (int i = 0; i < g; i++) {
		deck->deck_values[i] = gv;
		deck->deck_colors[i] = colors[green];
	}
	for (int j = 1; j < k + 1; j++) {
		for (int i = 0; i < o; i++) {
			if(i<k*o+g)
			deck->deck_values[i + ((o * (j - 1)) + g)] = values[i];
			deck->deck_colors[i + ((o * (j - 1)) + g)] = colors[j];
		}
	}
	return deck;
}

struct deck_type* deal_cards(int n, int cards_num, struct deck_type* deck) {
	struct deck_type* hand = new deck_type((cards_num) / n);
	static int player = 0;
	for (int i = 0; i <= cards_num / n; i++) {
		hand->deck_values[i] = deck->deck_values[player + (i * n)];
		hand->deck_colors[i] = deck->deck_colors[player + (i * n)];
	}
	player += 1;
	return hand;
}

int main() {
	enum kolory {
		green,
		blue,
		red,
		violet,
		yellow,
		white,
		black,
	};
	const char* colors[7] = { "green", "blue", "red", "violet", "yellow","white","black" };
	int n; // liczba graczy
	int k; // liczba kociolkow(kolorow-1)
	int g; // liczba zielonych kart
	int gv; // wartosc ziel kart
	int o; // liczba kart poszczegolnych kolorow
	int values[128]; // wartosci kart roznych kolorow
	std::cin>> n >> k >> g >> gv >> o;
	int cards_num = o * k + g;

	for (int i = 0; i < o; i++) {
		std::cin >> values[i];
	}
	struct deck_type* deck = create_deck(k, g, gv, o, values); // tworzenie talii kart


	cout << "active player = 1" << endl;
	cout << "players number = " << n << endl;
	for (int j = 1; j <= n; j++) {
		struct deck_type* hand = (deal_cards(n, cards_num, deck));
		cout << j << " player hand cards: ";
		if (j <= cards_num % n) { //jesli liczba kart % graczy != 0 to gracze beda mieli rozna ilosc kart, czyli wiecej dostanie gracz <= cards_num%n
			for (int i = 0; i <= (cards_num) / n; i++) {

				cout << hand->deck_values[i] << " " << hand->deck_colors[i] << " ";
			}
		}
		else {
			for (int i = 0; i <= ((cards_num) / n) - 1; i++) {

				cout << hand->deck_values[i] << " " << hand->deck_colors[i] << " ";
			}
		}
		cout << endl;
		cout << j << " player deck cards: " << endl;
	}
	for (int i = 1; i <= k; i++) {
		cout << i << " pile cards:" << endl;
	}
	delete deck;
}