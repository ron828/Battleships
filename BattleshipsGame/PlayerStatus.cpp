#include "PlayerStatus.h"

void PlayerStatus::initStatus(int playerNum)
{
	player = playerNum;
	score = sinkedShips = 0;
	outOfAttacks = false;
}

int PlayerStatus::getPlayer() const
{
	return player;
}

int PlayerStatus::getScore() const
{
	return score;
}

void PlayerStatus::addToScore(int amount)
{
	score += amount;
}

int PlayerStatus::getSinkedShips() const
{
	return sinkedShips;
}

void PlayerStatus::shipSank()
{
	sinkedShips++;
}

bool PlayerStatus::getIsOutOfAttacks() const
{
	return outOfAttacks;
}

void PlayerStatus::setOutOfAttacks()
{
	outOfAttacks = true;
}
