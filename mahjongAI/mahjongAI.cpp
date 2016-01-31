#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include "syanten.h"
using namespace std;

const int tileTypeCount = 34;
const int tileTypes[34] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 24, 25, 26, 27, 28, 29, 31, 32, 33, 34, 35, 36, 37 };
const int originWeight[38] = { -1, 1, 11, 12, 13, 14, 13, 12, 11, 1, -1, 1, 11, 12, 12, 13, 12, 12, 11, 1, -1, 1, 11, 12, 12, 13, 12, 12, 11, 1, -1, 0, 0, 0, 0, 3, 3, 3 };

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

int normalToRed(int tileType, int(&hand)[38], bool prefer) {
	if (tileType == 5 || tileType == 15 || tileType == 25) {
		if ((prefer || hand[tileType] == 1) && hand[tileType - 5] == 1) {
			tileType -= 5;
		}
	}
	return tileType;
}

int redToNormal(int tileType) {
	if (tileType == 0 || tileType == 10 || tileType == 20) {
		return tileType + 5;
	}
	else {
		return tileType;
	}
}

int directionToTileType(string& direction) {
	if (direction == "E") return 31;
	if (direction == "S") return 32;
	if (direction == "W") return 33;
	if (direction == "N") return 34;
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

void delGroup(int(&dst)[38], int(&src)[38]) {
	for (int i = 0; i < 38; i++) {
		dst[i] -= src[i];
	}
}

void ClearGroup(int (&dst)[38]) {
	memset(dst, 0, 38 * sizeof(int));
}

int* makeGroup(int(&dst)[38], int tile0, int tile1, int tile2) {
	memset(dst, 0, 38 * sizeof(int));
	addTile(dst, tile0);
	addTile(dst, tile1);
	addTile(dst, tile2);
	return dst;
}

int inputTile() {
	string tileStr;
	cin >> tileStr;
	int tileType = stringToTile(tileStr);
	cin.ignore(1);
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
	cin.ignore(1);
	return input == "True";
}

int getFuurosuu(int player) {
	cout << "fuurosuu " << player << endl;
	int input;
	cin >> input;
	cin.ignore(1);
	return input;
}

string getGameDirection() {
	cout << "direction 0" << endl;
	string input;
	cin >> input;
	cin.ignore(1);
	return input;
}

string getPlayerDirection(int player) {
	cout << "direction 1 " << player << endl;
	string input;
	cin >> input;
	cin.ignore(1);
	return input;
}

int calcKokusiDistance(int (&hand)[38]) {
	syanten.clear();
	syanten.set_tehai(hand);
	return syanten.KokusiSyanten();
}

int calcDistance(int(&hand)[38], int fuurosuu) {
	syanten.clear();
	syanten.set_tehai(hand);
	syanten.set_fuurosuu(fuurosuu);
	return syanten.AnySyanten() - fuurosuu * 2;
}

int isMentsu(int(&mentsu)[38]) {
	return calcDistance(mentsu, 0) == 6;
}

bool isYakuhai(int tileType, string& gameDirection, string& playerDirection) {
	if (tileType == directionToTileType(gameDirection)) return true;
	if (tileType == directionToTileType(playerDirection)) return true;
	if (tileType == 35 || tileType == 36 || tileType == 37) return true;
	return false;
}

bool hasYakuhai(int(&group)[38], string& gameDirection, string& playerDirection) {
	for (int i = 0; i < tileTypeCount; i++) {
		if (group[tileTypes[i]] >= 3 && isYakuhai(tileTypes[i], gameDirection, playerDirection)) {
			return true;
		}
	}
	return false;
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
	if (dora) res += 1000; //如果是dora则多算一个prob
	res += weight;
	return res;
}

int chooseDiscard(int (&hand)[38], int (&visible)[38], int (&dora)[38], int (&graveyard)[38], int (&weight)[38], int fuurosuu, int& resValue) {
	int dis = calcDistance(hand, fuurosuu);
	int res = -1, minValue = INT_MAX;
	for (int i = 0; i < tileTypeCount; i++) {
		if (hand[tileTypes[i]] > 0) {
			bool abort = false, furiten = false;
			int prob = 0;
			hand[tileTypes[i]]--;
			for (int j = 0; j < tileTypeCount; j++) {
				if (i != j && hand[tileTypes[j]] < 4) {
					hand[tileTypes[j]]++;
					int newDis = calcDistance(hand, fuurosuu);
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

	resValue = minValue;
	return res;
}

int chooseDiscard(int(&hand)[38], int(&visible)[38], int(&dora)[38], int(&graveyard)[38], int(&weight)[38], int fuurosuu) {
	int tmp;
	return chooseDiscard(hand, visible, dora, graveyard, weight, fuurosuu, tmp);
}

bool validateFuuro(int(&mentsu)[38], int(&hand)[38], int(&oldHand)[38], int fuurosuu) {
	for (int i = 0; i < tileTypeCount; i++) {
		if (hand[tileTypes[i]] < mentsu[tileTypes[i]]) {
			return false;
		}
	}

	if (!isMentsu(mentsu)) return false;

	int oldDis = calcDistance(oldHand, fuurosuu);

	delGroup(hand, mentsu);
	int dis = calcDistance(hand, fuurosuu + 1);
	addGroup(hand, mentsu);

	return dis < oldDis;
}

int main() {
	int hand[38];
	int fuuro[38];
	int visible[38];
	int dora[38];
	int graveyard[38];
	int weight[38];
	int fuurosuu;
	int tileType;
	int fromPlayer;
	string gameDirection;
	string playerDirection;
	string input;

	while (cin >> input) {
		if (input == "wait" || input == "draw") {
			tileType = inputTile();
			if (input == "wait") {
				cin >> fromPlayer;
				cin.ignore(1);
			}

			cout << "hand" << endl;
			inputGroup(hand);
			cout << "dora" << endl;
			inputGroup(dora);
			cout << "graveyard 0" << endl;
			inputGroup(graveyard);

			gameDirection = getGameDirection();
			playerDirection = getPlayerDirection(0);
			fuurosuu = getFuurosuu(0);

			memcpy(weight, originWeight, sizeof(weight));
			weight[directionToTileType(gameDirection)] = 2;
			weight[directionToTileType(playerDirection)] = 4;

			ClearGroup(visible);
			addGroup(visible, hand);
			addGroup(visible, dora);

			for (int i = 0; i < 4; i++) {
				cout << "graveyard " << i << " 1" << endl;
				inputGroup(visible, false);
				cout << "fuuro " << i << endl;
				inputGroup(visible, false);
			}
		}

		if (input == "wait") {
			int oldDis = calcDistance(hand, fuurosuu);
			int oldHand[38];
			memcpy(oldHand, hand, sizeof(hand));

			addTile(hand, tileType);
			int dis = calcDistance(hand, fuurosuu);

			if (dis == -1) {
				cout << "ron" << endl;
			}
			else {
				tileType = redToNormal(tileType);

				cout << "fuuro " << 0 << endl;
				inputGroup(fuuro);

				if (dis < oldDis && (hasYakuhai(hand, gameDirection, playerDirection) || hasYakuhai(fuuro, gameDirection, playerDirection))) {
					int discardTile = -1, minValue = INT_MAX, res, tmpDiscardTile, value;
					int candidate[4][38];
					makeGroup(candidate[0], tileType, tileType - 2, tileType - 1);
					makeGroup(candidate[1], tileType, tileType - 1, tileType + 1);
					makeGroup(candidate[2], tileType, tileType + 1, tileType + 2);
					makeGroup(candidate[3], tileType, tileType, tileType);

					for (int i = 0; i < 4; i++) {
						if (i != 3 && fromPlayer != 3) continue;
						if (validateFuuro(candidate[i], hand, oldHand, fuurosuu)) {
							delGroup(hand, candidate[i]);
							tmpDiscardTile = chooseDiscard(hand, visible, dora, graveyard, weight, fuurosuu + 1, value);
							addGroup(hand, candidate[i]);
							if (value < minValue) {
								discardTile = tmpDiscardTile;
								minValue = value;
								res = i;
							}
						}
					}

					if (discardTile != -1) {
						int tile0, tile1;
						switch (res) {
						case 0:
							tile0 = tileType - 2, tile1 = tileType - 1;
							break;
						case 1:
							tile0 = tileType - 1, tile1 = tileType + 1;
							break;
						case 2:
							tile0 = tileType + 1, tile1 = tileType + 2;
							break;
						case 3:
							tile0 = tileType, tile1 = tileType;
							break;
						}

						tile0 = normalToRed(tile0, oldHand, true);
						if (res != 3) tile1 = normalToRed(tile1, oldHand, true);

						if (res == 3) {
							cout << "pon " << tileToString(tile0) << " " << tileToString(tile1) << endl;
						}
						else {
							cout << "chii " << tileToString(tile0) << " " << tileToString(tile1) << endl;
						}

						discardTile = normalToRed(discardTile, oldHand, false);
						cout << "discard " << tileToString(discardTile) << endl;
					}
					else {
						cout << "pass" << endl;
					}
				}
				else {
					cout << "pass" << endl;
				}
			}
		}
		else if (input == "draw") {
			int dis = calcDistance(hand, fuurosuu);
			if (dis == -1) {
				cout << "tsumo" << endl;
			}
			else {
				int discardTile = chooseDiscard(hand, visible, dora, graveyard, weight, fuurosuu);
				if (discardTile == -1) {
					cout << "error" << endl;
				}
				else {
					discardTile = normalToRed(discardTile, hand, false);
					if (dis == 0 && fuurosuu == 0 && calcKokusiDistance(hand) != 0) {
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
		else if (input == "syanten") {
			fuurosuu = getFuurosuu(0);
			cin.ignore(1);
			inputGroup(hand);
			cout << calcDistance(hand, fuurosuu) << endl;
		}
	}
	return 0;
}