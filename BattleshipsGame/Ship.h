#pragma once

class Ship {
	int value;
	int health;
	int size;
	int player;
	char symbol;
	int squares[4];

public:
	Ship() : value(-1), health(-1), player(-1) {};
	~Ship() = default;
	void initShip(int shipValue, int shipHealth, int shipPlayer, char shipSymbol);
	bool isInitialized()const;
	const int getValue()const;
	const int getHealth()const;
	const int getPlayer()const;
	const int getSize()const;
	const char getSymbol()const;
	void addSquare(int squareIndex);
	const int getSquare(int i)const;

	const int shipHit(); // Decrement health by 1 and return updated health
};