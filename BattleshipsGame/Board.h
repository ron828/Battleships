#pragma once

#include <map>
#include "Ship.h"

class Board {
	int rows;
	int columns;
	static const int numOfShips = 18;
	Ship* ships[numOfShips] = { NULL };
	std::map<int, Ship*> boardMap;

	void initShipsArray();
	Ship* getUnusedShip()const; // Return first uninitilized ship
	int indexesToKey(int i, int j)const;

	/*
		Look for all connected ship squares originating from [i,j]
		in range and map them to ship
	*/
	void mapSquaresToShip(char** board, int i, int j, int value, int health, int player);

public:
	Board() = default;
	~Board();
	
	// Create boardMap from charBoard
	void initBoard(char** board, int rowsNum, int columnsNum);

	Ship* getShipInSquare(int i, int j);
	int getNumOfShips()const;
	void print()const;
};
