#pragma once

class PlayerStatus {
	int player;
	int score;
	int sinkedShips;
	bool outOfAttacks;

public:
	PlayerStatus() = default;
	~PlayerStatus() = default;
	void initStatus(int playerNum);
	int getPlayer()const;
	int getScore()const;
	void addToScore(int amount);
	int getSinkedShips()const;
	void shipSank();
	bool getIsOutOfAttacks()const;
	void setOutOfAttacks();
};
