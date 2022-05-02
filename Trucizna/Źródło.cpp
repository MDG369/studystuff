#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define CARDS_NUM 250
#define DECK_TXT "deck.txt"
#define INPUT_TXT "input.txt"
#define MAX_COLORS 7
#define MAX_ROUNDS 3
using namespace std;

enum colors {
	green,
	blue,
	red,
	violet,
	yellow,
	white,
	black,
};
const char* const colors[MAX_COLORS] = { "green", "blue", "red", "violet", "yellow","white","black" };

struct deck_type {
	int* deck_values;
	int* deck_colors;
	deck_type(int deck_size) {
		deck_values = new int[deck_size];
		deck_colors = new int[deck_size];
	}

	~deck_type() {
		delete[] deck_values;
		delete[] deck_colors;
	}
};

struct cauldron {
	int* card_count;
	bool* same_color;
	int* pile_value;
	int* color;
	int* deck_values;
	int* deck_colors;
	cauldron(int deck_size) {
		deck_values = new int[deck_size];
		deck_colors = new int[deck_size];
		card_count = new int[MAX_COLORS];
		same_color = new bool[MAX_COLORS];
		pile_value = new int[MAX_COLORS];
		color = new int[MAX_COLORS];
		for (int i = 0; i < MAX_COLORS; i++) {
			card_count[i] = 0;
			same_color[i] = 1;
			color[i] = 0;
			pile_value[i] = 0;
		}
	}

	~cauldron() {
		delete[] deck_values;
		delete[] deck_colors;
		delete[] card_count;
		delete[] same_color;
		delete[] color;
		delete[] pile_value;
	}

};

struct players_cards_type {
	int* hand_cards;
	int* front_cards;
	players_cards_type(int n) {
		hand_cards = new int[n];
		front_cards = new int[n];
		for (int i = 0; i < n; i++) {
			hand_cards[i] = 0;
			front_cards[i] = 0;
		}
	}
	~players_cards_type() {
		delete[] hand_cards;
		delete[] front_cards;
	}
};

struct deck_type* create_deck(int k, int g, int gv, int o, int values[128]);
struct deck_type* deal_cards(int n, int cards_num, struct deck_type* deck);
void count_colors(char* token, int tmp, deck_type* players, int& i, int& j, int* players_cards);
void get_variable(char  input[1000], char*& token, int& e, FILE* inputFile);
void shuffle_deck(int k, int o, int g, deck_type* deck);
void play_card(int active, players_cards_type* players_data, int k, cauldron* pile, int n, deck_type* players_hands, int e, deck_type* players_piles);
const char* number_to_color(int index);
void complete_deck(int n, int k, int g, int gv, int o, int e, int  values[128]);
void round_end(int n, int k, int* score, players_cards_type* players_data, deck_type* players_piles);

int main() {
	char choice;
	cout << "MENU" << endl;
	cout << "P - Przeprowadzenie rozgrywki" << endl;
	cout << "T - Tworzenie i rozdawanie talii" << endl;
	cin >> choice;
	srand(time(NULL));
	int active = 0;
	int n = 0; // liczba graczy
	int k = 0; // liczba kociolkow(kolorow-1)
	int g = 0; // liczba zielonych kart
	int gv = 0; // wartosc ziel kart
	int o = 0; // liczba kart poszczegolnych kolorow
	int e = 0; // explosion threshold
	int values[128]; // wartosci kart roznych kolorow
	char input[1000];
	char* token = strtok(input, " ");
	int round_number = 1;
	int score[21] = { 0 };
	FILE* inputFile;
	inputFile = fopen(DECK_TXT, "r+");
	if (inputFile == NULL)
	{
		cout << "Error opening file";
		return 0;
	}
		if (choice == 'P') {
			while (round_number <= MAX_ROUNDS) {
				n = 0; // liczba graczy
				k = 0; // liczba kociolkow(kolorow-1)
				g = 0; // liczba zielonych kart
				gv = 0; // wartosc ziel kart
				o = 0; // liczba kart poszczegolnych kolorow
				e = 0; // explosion threshold		
				char* token = strtok(input, " ");
				int tmp = 0;
				inputFile = fopen(DECK_TXT, "r+");
				get_variable(input, token, active, inputFile);
				get_variable(input, token, n, inputFile);
				get_variable(input, token, e, inputFile);
				cauldron* pile = new cauldron(CARDS_NUM);
				players_cards_type* players_data = new players_cards_type(n + 1);
				deck_type* players_hands = new deck_type(CARDS_NUM);
				deck_type* players_piles = new deck_type(CARDS_NUM);
				int diffgv = 0;
				int card_nr = 0;
				int pile_card_nr = 0;
				for (int i = 1; i <= n; i++) {
					fgets(input, 1000, inputFile);
					size_t ln = strlen(input) - 1;
					if (input[ln] == '\n') {
						input[ln] = '\0';
					}
					strcat(input, " ");
					int tmp = 0;
					token = strtok(input, " ");
					while (token != NULL) {
						if (*token >= '0' && *token <= '9') {
							tmp = atoi(token);
						}
						if (strcmp(token, "green") == 0 && gv == 0)
						{
							gv = tmp;
							g += 1;
							players_data->hand_cards[i] += 1;
							players_hands->deck_values[card_nr] = tmp;
							players_hands->deck_colors[card_nr] = green;
							card_nr++;
						}
						else if (strcmp(token, "green") == 0 && gv != 0) {
							g += 1;
							players_data->hand_cards[i] += 1;
							players_hands->deck_values[card_nr] = tmp;
							players_hands->deck_colors[card_nr] = green;
							card_nr++;
							if (tmp != gv) {
								diffgv = 1;
							}
						}
						count_colors(token, tmp, players_hands, i, card_nr, players_data->hand_cards);
						token = strtok(NULL, " ");
					}

					fgets(input, 1000, inputFile);
					ln = strlen(input) - 1;
					if (input[ln] == '\n') {
						input[ln] = '\0';
					}
					token = strtok(input, " ");

					while (token != NULL) {
						if (*token >= '0' && *token <= '9') {
							tmp = atoi(token);
						}
						if (strcmp(token, "green") == 0 && gv == 0)
						{
							gv = tmp;
							g += 1;
							players_data->front_cards[i] += 1;
							players_piles->deck_values[pile_card_nr] = tmp;
							players_piles->deck_colors[pile_card_nr] = green;
							pile_card_nr++;
						}
						else if (strcmp(token, "green") == 0 && gv != 0) {
							g += 1;
							players_data->front_cards[i] += 1;
							players_piles->deck_values[pile_card_nr] = tmp;
							players_piles->deck_colors[pile_card_nr] = green;
							pile_card_nr++;
							if (tmp != gv) {
								diffgv = 1;
							}
						}
						count_colors(token, tmp, players_piles, i, pile_card_nr, players_data->front_cards);
						token = strtok(NULL, " ");
					}
				}
				card_nr = 0;
				while (!feof(inputFile)) {
					fgets(input, 1000, inputFile);
					size_t ln = strlen(input) - 1;
					if (input[ln] == '\n') {
						input[ln] = '\0';
					}
					token = strtok(input, " ");
					pile->same_color[k] = true;
					int tmp = 0;
					while (token != NULL) {
						if (*token >= '0' && *token <= '9') {
							tmp = atoi(token);
						}
						if (strcmp(token, "green") == 0 && gv == 0)
						{
							gv = tmp;
							g += 1;
							pile->card_count[k] += 1;
							pile->pile_value[k] += tmp;
							pile->deck_values[card_nr] = tmp;
							pile->deck_colors[card_nr] = green;
							card_nr++;
						}
						else if (strcmp(token, "green") == 0 && gv != 0) {
							g += 1;
							pile->card_count[k] += 1;
							pile->pile_value[k] += tmp;
							pile->deck_values[card_nr] = tmp;
							pile->deck_colors[card_nr] = green;
							card_nr++;
							if (tmp != gv) {
								diffgv = 1;
							}
						}
						if (strcmp(token, "blue") == 0) {
							pile->card_count[k] += 1;
							pile->pile_value[k] += tmp;
							pile->deck_values[card_nr] = tmp;
							pile->deck_colors[card_nr] = blue;
							card_nr++;
							if (pile->color[k] == NULL) {
								pile->color[k] = blue;
							}
							if (pile->color[k] != blue) {
								pile->same_color[k] = false;
							}
						}
						if (strcmp(token, "red") == 0) {
							pile->card_count[k] += 1;
							pile->pile_value[k] += tmp;
							pile->deck_values[card_nr] = tmp;
							pile->deck_colors[card_nr] = red;
							card_nr++;
							if (pile->color[k] == NULL) {
								pile->color[k] = red;
							}
							if (pile->color[k] != red) {
								pile->same_color[k] = false;
							}
						}
						if (strcmp(token, "violet") == 0) {
							pile->card_count[k] += 1;
							pile->pile_value[k] += tmp;
							pile->deck_values[card_nr] = tmp;
							pile->deck_colors[card_nr] = violet;
							card_nr++;
							if (pile->color[k] == NULL) {
								pile->color[k] = violet;
							}
							if (pile->color[k] != violet) {
								pile->same_color[k] = false;
							}
						}
						if (strcmp(token, "yellow") == 0) {
							pile->card_count[k] += 1;
							pile->pile_value[k] += tmp;
							pile->deck_values[card_nr] = tmp;
							pile->deck_colors[card_nr] = yellow;
							card_nr++;
							if (pile->color[k] == NULL) {
								pile->color[k] = yellow;
							}
							if (pile->color[k] != yellow) {
								pile->same_color[k] = false;
							}
						}
						if (strcmp(token, "white") == 0) {
							pile->card_count[k] += 1;
							pile->pile_value[k] += tmp;
							pile->deck_values[card_nr] = tmp;
							pile->deck_colors[card_nr] = white;
							card_nr++;
							if (pile->color[k] == NULL) {
								pile->color[k] = white;
							}
							if (pile->color[k] != white) {
								pile->same_color[k] = false;
							}
						}
						if (strcmp(token, "black") == 0) {
							pile->card_count[k] += 1;
							pile->pile_value[k] += tmp;
							pile->deck_values[card_nr] = tmp;
							pile->deck_colors[card_nr] = black;
							card_nr++;
							if (pile->color[k] == NULL) {
								pile->color[k] = black;
							}
							if (pile->color[k] != black) {
								pile->same_color[k] = false;
							}
						}
						token = strtok(NULL, " ");
					}
					k++;
				};
				fclose(inputFile);
				inputFile = fopen(DECK_TXT, "w");
				int players_cards_num = 0;
				for (int i = 1; i <= n; i++) {
					players_cards_num += players_data->hand_cards[i];
				}

				if (players_cards_num == 0) {						//obs³uga koñca rundy
					round_end(n, k, score, players_data, players_piles);
					cout << "end of round " << round_number << endl;;
					round_number += 1;
					if (round_number <= 3) {
						cout << "Enter deck data: " << endl;
						complete_deck(n, k, g, gv, o, e, values);
					}
				}
				else {
					play_card(active, players_data, k, pile, n, players_hands, e, players_piles);

					active += 1;
					if (active > n) {
						active = 1;
					}
					fprintf(inputFile, "active player = %d\nplayers number = %d\nexplosion threshold = %d\n", active, n, e);
					cout << "active player = " << active << endl;
					cout << "players number = " << n << endl;
					cout << "explosion threshold = " << e << endl;
					int count = 0;
					int pile_count = 0;
					for (int j = 1; j <= n; j++) {
						for (int bug = 0; bug < k; bug++) {
							if (pile->pile_value[bug] == e) {
								cout << "pile_value == e" << endl;
							}
						}
						cout << j << " player hand cards: ";
						fprintf(inputFile, "%d player hand cards: ", j);
						for (int i = 0; i < players_data->hand_cards[j]; i++, count++) {
							cout << players_hands->deck_values[count] << " " << players_hands->deck_colors[count] << " ";
							fprintf(inputFile, "%d %s ", players_hands->deck_values[count], colors[players_hands->deck_colors[count]]);
						}
						cout << endl;
						cout << j << " player deck cards: ";
						fprintf(inputFile, "\n%d player deck cards: ", j);
						for (int i = 0; i < players_data->front_cards[j]; i++, pile_count++) {
							cout << players_piles->deck_values[pile_count] << " " << players_piles->deck_colors[pile_count] << " ";
							fprintf(inputFile, "%d %s ", players_piles->deck_values[pile_count], colors[players_piles->deck_colors[pile_count]]);
						}
						cout << endl;
						fprintf(inputFile, "\n");
					}
					count = 0;
					for (int j = 0; j < k; j++) {
						cout << j + 1 << " pile cards:";
						fprintf(inputFile, "%d pile cards:", j + 1);
						for (int i = 0; i < pile->card_count[j]; i++, count++) {
							cout << " " << pile->deck_values[count] << " " << pile->deck_colors[count];
							fprintf(inputFile, " %d %s", pile->deck_values[count], colors[pile->deck_colors[count]]);

						}
						cout << endl;
						if (j < k - 1) {
							fprintf(inputFile, "\n");
						}
					}
					int players_cards_num = 0;
					for (int i = 1; i <= n; i++) {
						players_cards_num += players_data->hand_cards[i];
					}

				}
				fclose(inputFile);
				delete players_hands;
				delete players_piles;
				delete pile;
				delete players_data;
			}
	}
		/* ########## tworzenie i rozdawanie talii ########## */
	if (choice == 'T') {
		complete_deck(n, k, g, gv, o, e, values);
	}
}

void complete_deck(int n, int k, int g, int gv, int  o, int e, int  values[128])
{
	cin >> n >> k >> g >> gv >> o >> e;
	for (int i = 0; i < o; i++) {
		std::cin >> values[i];
	}
	struct deck_type* deck = create_deck(k, g, gv, o, values); // tworzenie talii kart
	shuffle_deck(k, o, g, deck);
	FILE* deckFile = fopen(DECK_TXT, "w");
	fprintf(deckFile, "active player = 1 \n");
	fprintf(deckFile, "players number = %d \n", n);
	fprintf(deckFile, "explosion threshold = %d \n", e);

	for (int j = 1; j <= n; j++) {
		fprintf(deckFile, "%d player hand cards: ", j);
		struct deck_type* hand = (deal_cards(n, o * k + g , deck));
		if (j <= (o * k + g) % n) { //jesli (liczba kart % graczy) != 0 to gracze beda mieli rozna ilosc kart, czyli wiecej dostanie gracz <= cards_num%n
			for (int i = 0; i <= (o * k + g) / n; i++) {
				fprintf(deckFile, "%d %s ", hand->deck_values[i], colors[hand->deck_colors[i]]);
			}
		}
		else {
			for (int i = 0; i <= ((o * k + g) / n) - 1; i++) {
				fprintf(deckFile, "%d %s ", hand->deck_values[i], colors[hand->deck_colors[i]]);
			}
		}
		fprintf(deckFile, "\n");
		fprintf(deckFile, "%d player deck cards: \n", j);
	}
	for (int i = 1; i <= k; i++) {
		if (i != k) {
			fprintf(deckFile, "%d pile cards:\n", i);
		}
		else {
			fprintf(deckFile, "%d pile cards:", i);
		}
	}
	fclose(deckFile);
	delete deck;

}

const char* number_to_color(int index) {
	return colors[index];
}

void round_end(int n, int k, int* score, players_cards_type* players_data, deck_type* players_piles)
{
	FILE* scoreFile = fopen("score.txt", "w");
	if (scoreFile == NULL)
	{
		cout << "Error opening file";
		return ;
	}
	int final_cards[CARDS_NUM] = { 0 };
	int safety[MAX_COLORS] = { 0 };
	for (int i = 0; i < n * k; i++) {
		final_cards[i] = 0;
	}
	for (int i = 0; i < k; i++) {
		safety[i] = 0;
	}
	int counter = 0;
	for (int j = 1; j <= n; j++) { // dla kazdego gracza nastêpuje podliczanie liczb kart poszczegolnych kolorów
		for (int i = 0; i < players_data->front_cards[j]; i++, counter++) { // dla kazdej karty
			for (int c = 0; c <= k; c++) {
				if (players_piles->deck_colors[counter] == c) {
					final_cards[((j - 1) * MAX_COLORS) + c] += 1;
				}
			}
		}
	}
	for (int c = 1; c <= k; c++) { // obliczanie ochrony przed danym kolorem karty
		int most_cards = 0;
		for (int j = 0; j < n; j++) {
			if (final_cards[(j * MAX_COLORS) + c] > most_cards) {
				safety[c] = j + 1;
				most_cards = final_cards[(j * MAX_COLORS) + c];
			}
			else if (final_cards[(j * MAX_COLORS) + c] == most_cards) {
				safety[c] = 0;
			}
		}
	}
	for (int i = 1; i <= k; ++i) {
		if (safety[i] != 0) {
			cout << "Na kolor " << colors[i] << " odporny jest gracz " << safety[i] << endl;
		}
	}
	for (int j = 0; j < n; j++) {
		for (int c = 0; c < k; c++) {
			for (int i = 0; i < final_cards[(j * MAX_COLORS) + c]; i++) {
				if (c == 0) {
					score[j] += 2;
				}
				else if (safety[c] != (j + 1)) {
					score[j] += 1;
				}
			}
		}
	}
	for (int i = 0; i < n; i++) {
		cout << "Wynik gracza " << i + 1 << " = " << score[i] << endl;
		fprintf(scoreFile, "Wynik gracza %d = %d\n", i + 1, score[i]);
	}
	fclose(scoreFile);
}

void play_card(int active, players_cards_type* players_data, int k, cauldron* pile, int n, deck_type* players_hands, int e, deck_type* players_piles)
{
	int card_play = 0;
	for (int i = active; i > 1; i--) {
		card_play += players_data->hand_cards[i - 1];
	}
	int pile_cards_num = 0;
	for (int i = 0; i < k; i++) {
		pile_cards_num += pile->card_count[i];
	}
	int hand_cards_num = 0;
	for (int i = 1; i <= n; i++) {
		hand_cards_num += players_data->hand_cards[i];
	}
	int players_pile_cards_num = 0;
	for (int i = 1; i <= n; i++) {
		players_pile_cards_num += players_data->front_cards[i];
	}
	int which_pile = -1;
	int prv_cards = 0;
	for (int c = 0; c < k; c++) {
		if (players_hands->deck_colors[card_play] == green) { // jesli kolor zagranej karty to zielony to wybieramy pierwszy kocio³ek
			which_pile = 0;
		}
		if (players_hands->deck_colors[card_play] == c + 1) {
			for (int i = 0; i < k; i++) {	// spr, czy jest kocio³ek odpowiedniego koloru
				if (pile->color[i] == c + 1) {
					which_pile = i;
					break;
				}
			}
			for (int i = 0; (i < k) && which_pile == -1; i++) { // jesli nie ma szukamy pustego kocio³ka
				if (pile->color[i] == NULL) {
					which_pile = i;
					break;
				}
			}
		}
	}

	for (int i = which_pile; i >= 0; i--) {
		prv_cards += pile->card_count[i];
	}
	for (int i = pile_cards_num; i >= prv_cards; i--) {
		pile->deck_colors[i + 1] = pile->deck_colors[i];
		pile->deck_values[i + 1] = pile->deck_values[i];
	}
	pile->card_count[which_pile] += 1;
	pile->deck_colors[prv_cards] = players_hands->deck_colors[card_play];
	pile->deck_values[prv_cards] = players_hands->deck_values[card_play];
	pile->pile_value[which_pile] += players_hands->deck_values[card_play];
	pile_cards_num += 1;
	hand_cards_num -= 1;
	for (int i = card_play; i < hand_cards_num; i++) { // "cofnij" karty
		players_hands->deck_colors[i] = players_hands->deck_colors[i + 1];
		players_hands->deck_values[i] = players_hands->deck_values[i + 1];
	}
	players_data->hand_cards[active] -= 1;
	if (pile->pile_value[which_pile] > e) { // gdy kocio³ek wybuchnie
		prv_cards = 0;
		for (int i = active; i > 0; i--) { // licz poprzednie karty w stosach graczy
			prv_cards += players_data->front_cards[i];
		}
		int prv_pile_cards = 0;
		for (int i = which_pile - 1; i >= 0; i--) {
			prv_pile_cards += pile->card_count[i];
		}

		for (int i = players_pile_cards_num; i >= prv_cards; i--) { // rob miejsce na nowe karty w stosach
			players_piles->deck_colors[i + pile->card_count[which_pile]] = players_piles->deck_colors[i];
			players_piles->deck_values[i + pile->card_count[which_pile]] = players_piles->deck_values[i];
		}
		for (int i = 0; i < pile->card_count[which_pile]; i++) { // dodaj karty z kocio³ka na stos aktywnego gracza
			players_piles->deck_colors[i + prv_cards] = pile->deck_colors[i + prv_pile_cards];
			players_piles->deck_values[i + prv_cards] = pile->deck_values[i + prv_pile_cards];
		}
		for (int j = 0; j < pile->card_count[which_pile]; j++) { // przesun karty w kocio³kach o tyle miejsc ile kart zosta³o odjêtych
			for (int i = prv_pile_cards; i < pile_cards_num; i++) {
				pile->deck_values[i] = pile->deck_values[i + 1];
				pile->deck_colors[i] = pile->deck_colors[i + 1];
			}
		}
		players_data->front_cards[active] += pile->card_count[which_pile];
		pile->card_count[which_pile] = 0;
		pile->pile_value[which_pile] = 0;
	}
}

void count_colors(char* token, int tmp, deck_type* players, int& i, int& j, int* players_cards)
{
	if (strcmp(token, "blue") == 0) {
		players_cards[i] += 1;
		players->deck_values[j] = tmp;
		players->deck_colors[j] = blue;
		j++;
	}
	if (strcmp(token, "red") == 0) {
		players_cards[i] += 1;
		players->deck_values[j] = tmp;
		players->deck_colors[j] = red;
		j++;
	}
	if (strcmp(token, "violet") == 0) {
		players_cards[i] += 1;
		players->deck_values[j] = tmp;
		players->deck_colors[j] = violet;
		j++;
	}
	if (strcmp(token, "yellow") == 0) {
		players_cards[i] += 1;
		players->deck_values[j] = tmp;
		players->deck_colors[j] = yellow;
		j++;
	}
	if (strcmp(token, "white") == 0) {
		players_cards[i] += 1;
		players->deck_values[j] = tmp;
		players->deck_colors[j] = white;
		j++;
	}
	if (strcmp(token, "black") == 0) {
		players_cards[i] += 1;
		players->deck_values[j] = tmp;
		players->deck_colors[j] = black;
		j++;
	}
}

deck_type* create_deck(int k, int g, int gv, int o, int values[128]) {
	struct deck_type* deck = new deck_type(k * o + g);


	for (int i = 0; i < g; i++) {
		deck->deck_values[i] = gv;
		deck->deck_colors[i] = green;
	}
	for (int j = 1; j < k + 1; j++) {
		for (int i = 0; i < o; i++) {
			if (i < k * o + g)
				deck->deck_values[i + ((o * (j - 1)) + g)] = values[i];
			deck->deck_colors[i + ((o * (j - 1)) + g)] = j;
		}
	}
	return deck;
}

void shuffle_deck(int k, int o, int g, deck_type* deck)
{
	for (int i = 0; i < k * o + g; i++) {
		int karta = rand() % (k * o + g);
		int karta2 = rand() % (k * o + g);
		int tmp_clr = deck->deck_colors[karta2];
		int tmp_val = deck->deck_values[karta2];
		deck->deck_values[karta2] = deck->deck_values[karta];
		deck->deck_colors[karta2] = deck->deck_colors[karta];
		deck->deck_values[karta] = tmp_val;
		deck->deck_colors[karta] = tmp_clr;
	}
}

deck_type* deal_cards(int n, int cards_num, struct deck_type* deck) {
	deck_type* hand = new deck_type((cards_num) / n);
	static int player = 0;
	for (int i = 0; i <= cards_num / n; i++) {
		hand->deck_values[i] = deck->deck_values[player + (i * n)];
		hand->deck_colors[i] = deck->deck_colors[player + (i * n)];
	}
	player += 1;
	if (player >= n) {
		player = 0;
	}
	return hand;
}

void get_variable(char  input[1000], char*& token, int& e, FILE* inputFile)
{
	fgets(input, 1000, inputFile);
	token = strtok(input, " ");
	while (token != NULL) {
		if (*token >= '0' && *token <= '9') {
			e = atoi(token);
		}
		token = strtok(NULL, " ");
	}
}