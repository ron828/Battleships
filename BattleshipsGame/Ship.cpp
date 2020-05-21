#include "Ship.h"

void Ship::initShip(int shipValue, int shipHealth, int shipPlayer, char shipSymbol)
{
	value = shipValue;
	size = health = shipHealth;
	player = shipPlayer;
	symbol = shipSymbol;
	squares[0] = squares[1] = squares[2] = squares[3] = 0;
}

bool Ship::isInitialized() const
{
	if (value == -1) return false;
	return true;
}

const int Ship::getValue() const
{
	return value;
}

const int Ship::getHealth() const
{
	return health;
}

const int Ship::getPlayer() const
{
	return player;
}

const int Ship::getSize() const
{
	return size;
}

const char Ship::getSymbol() const
{
	return symbol;
}

void Ship::addSquare(int squareIndex)
{
	for (int i = 0; i < 4; i++) {
		if (squares[i] == 0) {
			squares[i] = squareIndex;
			break;
		}
	}
}

const int Ship::getSquare(int i) const
{
	if (i >= 0 && i <= 3) return squares[i];
	return 0;
}

const int Ship::shipHit()
{
	health--;
	return health;
}
