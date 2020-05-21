#pragma once
#include <fstream>
#include <random>
#include <vector>
#include <unordered_map>
#include "IBattleshipGameAlgo.h"
#include "CommonUtils.h"


class EngineZero : public IBattleshipGameAlgo {
	int myPlayerNum;
	std::unordered_map<int, std::pair<int, int>> attacksMap;
	std::unordered_map<int, std::pair<int, int>> secondaryAttacksMap;
	std::vector<std::pair<int, int>> enemyShip;
	int numOfRows, numOfColumns;
	enum HitModeDirection
	{
		OFF, LEFT, RIGHT, UP, DOWN
	};
	int directionChangedCounter;
	HitModeDirection hitMode;
	std::pair<int, int> hitModeAttack, hitModeStartingPoint;
	std::random_device rd;
	std::mt19937 rng;

	// Returns true if adjacent squares to <i,j> are empty
	bool adjacentSquaresEmpty(const char** board, int rowsNum, int columnsNum, int i, int j)const;

	// Removes from attacksMap squares around input square
	void removeAdjacentSquares(std::pair<int, int> square);

	// Enter hit mode (enemy ship hit, try hitting rest of ship)
	void enterHitMode(int i, int j);

	// Change attack direction in hit mode
	void nextAttackDirection();

	// Returns random number between min and max
	int getRandom(int min, int max);

	// Returns true iff attack is valid
	bool isAttackValid(std::pair<int, int> attack)const;

	// Move attack to secondary attacks map
	void moveToSecondaryAttacks(std::pair<int, int> attack);

	// Remove attack from attacks maps
	void removeAttack(int row, int col);

	// Returns true iff square is in one of attacks maps
	bool isSquareInAttacksMaps(int row, int col)const;

public:
	EngineZero() : rng(rd()) {};
	~EngineZero() = default;
	void setBoard(int player, const char** board, int numRows, int numCols) override;
	bool init(const std::string& path) override;
	std::pair<int, int> attack() override;
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
};
