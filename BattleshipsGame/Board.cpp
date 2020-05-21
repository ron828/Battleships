#include <iostream>
#include <string>
#include "Board.h"

using namespace std;

void Board::initShipsArray()
{
	for (int i = 0; i < numOfShips; i++) {
		ships[i] = new Ship();
	}
}

int Board::indexesToKey(int i, int j) const
{
	return i*columns + j;
}

Ship * Board::getUnusedShip()const
{
	for (int i = 0; i < numOfShips; i++) {
		if (!(ships[i]->isInitialized())) {
			return ships[i];
		}
	}
	return nullptr;
}

void Board::mapSquaresToShip(char** board, int i, int j, int value, int health, int player)
{
	char c = board[i][j];
	int key = indexesToKey(i, j);
	Ship* ship = getUnusedShip();
	ship->initShip(value, health, player, c);
	ship->addSquare(key);
	boardMap[key] = ship;

	// Ship takes more than one square, find the others
	if (health > 1) {
		// Determine in which direction to go - right or down
		if (j + health <= columns && board[i][j + 1] == c) {
			for (int k = j+1; k < j+health; k++) {
				key = indexesToKey(i, k);
				boardMap[key] = ship;
				ship->addSquare(key);
			}
		}

		// Go down
		else {
			for (int k = i + 1; k < i+health; k++) {
				key = indexesToKey(k, j);
				boardMap[key] = ship;
				ship->addSquare(key);
			}
		}
	}
}

Board::~Board()
{
	for (int i = 0; i < numOfShips; i++) {
		delete ships[i];
	}
}

void Board::initBoard(char ** board, int rowsNum, int columnsNum)
{
	rows = rowsNum;
	columns = columnsNum;
	initShipsArray();
	int key;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {

			key = indexesToKey(i, j);

			// Already belongs to a ship, skip
			if (boardMap.count(key) > 0) {
				continue;
			}
			else if (board[i][j] == 'B') {
				mapSquaresToShip(board, i, j, 2, 1, 0);
			}
			else if (board[i][j] == 'P') {
				mapSquaresToShip(board, i, j, 3, 2, 0);
			}
			else if (board[i][j] == 'M') {
				mapSquaresToShip(board, i, j, 7, 3, 0);
			}
			else if (board[i][j] == 'D') {
				mapSquaresToShip(board, i, j, 8, 4, 0);
			}
			else if (board[i][j] == 'b') {
				mapSquaresToShip(board, i, j, 2, 1, 1);
			}
			else if (board[i][j] == 'p') {
				mapSquaresToShip(board, i, j, 3, 2, 1);
			}
			else if (board[i][j] == 'm') {
				mapSquaresToShip(board, i, j, 7, 3, 1);
			}
			else if (board[i][j] == 'd') {
				mapSquaresToShip(board, i, j, 8, 4, 1);
			}

			// Unkown character, ignore
			else {
				continue;
			}
		}
	}
}

Ship * Board::getShipInSquare(int i, int j)
{
	int key = indexesToKey(i, j);
	if (boardMap.count(key) > 0) return boardMap[key];
	return nullptr;
}

int Board::getNumOfShips() const
{
	return numOfShips;
}

void Board::print() const
{
	for (int i = 0; i < numOfShips; i++) {
		cout << "Ship #" + to_string(i) + ":" << endl;
		cout << "Player: " + to_string(ships[i]->getPlayer()) << endl;
		cout << "Health: " + to_string(ships[i]->getHealth()) << endl;
		cout << "Value: " + to_string(ships[i]->getValue()) << endl;
		cout << endl;
	}
}
