#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include "syanten.h"
using namespace std;

const int tileTypeCount = 34;
const int tileTypes[34] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 24, 25, 26, 27, 28, 29, 31, 32, 33, 34, 35, 36, 37 };
const int weight[38] = { -1, 10, 11, 12, 13, 14, 13, 12, 11, 10, -1, 10, 11, 12, 12, 13, 12, 12, 11, 10, -1, 10, 11, 12, 12, 13, 12, 12, 11, 10, -1, 0, 0, 0, 0, 0, 0, 0 };

Syanten syanten;

int stringToTile(string str) {
	int res;

	if (str[1] == 'm') {
		res = str[0] - '0';
	}
	else if (str[1] == 'p') {
		res = 10 + str[0] - '0';
	}
	else if (str[1] == 's') {
		res = 20 + str[0] - '0';
	}
	else if (str[1] == 'z') {
		res = 30 + str[0] - '0';
	}

	//use 0 to represent red tiles instead of 5
	if (str.length() >= 3 && str[2] == '*') {
		res -= 5;
	}

	return res;
}

string tileToString(int tileType) {
	int num = tileType % 10;
	char color;
	bool isRed = false;

	if (num == 0) {
		isRed = true;
		num = 5;
	}

	switch (tileType / 10)
	{
	case 0:
		color = 'm';
		break;
	case 1:
		color = 'p';
		break;
	case 2:
		color = 's';
		break;
	case 3:
		color = 'z';
		break;
	default:
		break;
	}

	string res;
	res += num + '0';
	res += color;
	if (isRed) {
		res += '*';
	}

	return res;
}

void addTile(int (&dst)[38], int tileType) {
	if (tileType % 10 == 0) {
		dst[tileType]++;
		dst[tileType + 5]++;
	}
	else {
		dst[tileType]++;
	}
}

void addGroup(int(&dst)[38], int(&src)[38]) {
	for (int i = 0; i < 38; i++) {
		dst[i] += src[i];
	}
}

void ClearGroup(int (&dst)[38]) {
	memset(dst, 0, 38 * sizeof(int));
}

int inputTile() {
	string tileStr;
	cin >> tileStr;
	int tileType = stringToTile(tileStr);
	return tileType;
}

void inputGroup(int (&dst)[38], bool clear = true) {
	if (clear) {
		ClearGroup(dst);
	}

	string input, tileStr;
	getline(cin, input);
	stringstream stream;
	stream << input;

	while (stream >> tileStr) {
		int tileType = stringToTile(tileStr);
		addTile(dst, tileType);
	}
}

bool isReached(int player) {
	cout << "reached " << player << endl;
	string input;
	cin >> input;
	cin.ignore(1024, '\n');
	return input == "True";
}

int calcDistance(int (&hand)[38]) {
	syanten.clear();
	syanten.set_tehai(hand);
	return syanten.NormalSyanten();
}

int prevSibling(int tileType) {
	if (tileType == 1 || tileType == 11 || tileType == 21) {
		return tileType - 1 + 9;
	}
	else if (tileType == 31) {
		return 34;
	}
	else if (tileType == 35) {
		return 37;
	}
	else {
		return tileType - 1;
	}
}

int eval(int prob, int dora, int weight, bool furiten, int dis) {
	int res = 0;
	if (dis > 0 || dis == 0 && !furiten) {
		res += 100000;
	}
	res -= prob * 1000;
	res += dora * 100;
	res += weight;
	return res;
}

int chooseDiscard(int (&hand)[38], int (&visible)[38], int (&dora)[38], int (&graveyard)[38]) {
	int dis = calcDistance(hand);
	int res = -1, minValue = INT_MAX;
	for (int i = 0; i < tileTypeCount; i++) {
		if (hand[tileTypes[i]] > 0) {
			bool abort = false, furiten = false;
			int prob = 0;
			hand[tileTypes[i]]--;
			for (int j = 0; j < tileTypeCount; j++) {
				if (i != j && hand[tileTypes[j]] < 4) {
					hand[tileTypes[j]]++;
					int newDis = calcDistance(hand);
					if (newDis > dis) {
						abort = true;
						hand[tileTypes[j]]--;
						break;
					}
					else if (newDis < dis) {
						if (graveyard[tileTypes[j]] == 0) {
							prob += 4 - visible[tileTypes[j]];
						}
						else {
							furiten = true;
						}
					}
					hand[tileTypes[j]]--;
				}
			}
			if (abort) {
				hand[tileTypes[i]]++;
				continue;
			}
			int value = eval(prob, dora[prevSibling(tileTypes[i])], weight[tileTypes[i]], furiten, dis);
			if (value < minValue) {
				res = tileTypes[i];
				minValue = value;
			}
			hand[tileTypes[i]]++;
		}
	}

	//choose red tile if there is no other choices
	if (res == 5 || res == 15 || res == 25) {
		if (hand[res] == 1 && hand[res - 5] == 1) {
			res = res - 5;
		}
	}

	return res;
}

int main() {
	int hand[38];
	int visible[38];
	int dora[38];
	int graveyard[38];
	string input;

	while (cin >> input) {
		if (input == "wait") {
			int tileType = inputTile();
			cin.ignore(1024, '\n');

			cout << "hand" << endl;
			inputGroup(hand);
			addTile(hand, tileType);

			int dis = calcDistance(hand);
			if (dis == -1) {
				cout << "ron" << endl;
			}
			else {
				cout << "pass" << endl;
			}
		}
		else if (input == "draw") {
			int tileType = inputTile();
			cin.ignore(1024, '\n');

			cout << "hand" << endl;
			inputGroup(hand);
			cout << "dora" << endl;
			inputGroup(dora);
			cout << "graveyard 0" << endl;
			inputGroup(graveyard);

			ClearGroup(visible);
			addGroup(visible, hand);
			addGroup(visible, dora);
			addGroup(visible, graveyard);

			for (int i = 0; i < 4; i++) {
				cout << "graveyard " << i << " 1"<< endl;
				inputGroup(visible, false);
				cout << "fuuro " << i << endl;
				inputGroup(visible, false);
			}

			int dis = calcDistance(hand);
			if (dis == -1) {
				cout << "tsumo" << endl;
			}
			else {
				int discardTile = chooseDiscard(hand, visible, dora, graveyard);
				if (discardTile == -1) {
					cout << "error" << endl;
				}
				else {
					if (dis == 0) {
						if (!isReached(0)) {
							cout << "reach " << tileToString(discardTile) << endl;
							cout << "discard " << tileToString(discardTile) << endl;
						}
						else {
							cout << "tsumokiri" << endl;
						}
					}
					else {
						cout << "discard " << tileToString(discardTile) << endl;
					}				
				}
			}			
		}
	}
	return 0;
}