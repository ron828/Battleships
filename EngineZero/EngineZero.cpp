#include "EngineZero.h"

bool EngineZero::adjacentSquaresEmpty(const char** board, int rowsNum, int columnsNum, int i, int j) const
{
	if (i > 0) {
		if (CommonUtils::ShipUtils::isShip(board[i - 1][j])) return false;
	}

	if (i + 1 < rowsNum) {
		if (CommonUtils::ShipUtils::isShip(board[i + 1][j])) return false;
	}

	if (j > 0) {
		if (CommonUtils::ShipUtils::isShip(board[i][j - 1])) return false;
	}

	if (j + 1 < columnsNum) {
		if (CommonUtils::ShipUtils::isShip(board[i][j + 1])) return false;
	}

	return true;
}

void EngineZero::removeAdjacentSquares(std::pair<int, int> square)
{
	if (isAttackValid(std::pair<int, int>(square.first + 1, square.second)))
		removeAttack(square.first + 1, square.second);

	if (isAttackValid(std::pair<int, int>(square.first - 1, square.second)))
		removeAttack(square.first - 1, square.second);

	if (isAttackValid(std::pair<int, int>(square.first, square.second + 1)))
		removeAttack(square.first, square.second + 1);

	if (isAttackValid(std::pair<int, int>(square.first, square.second - 1)))
		removeAttack(square.first, square.second - 1);
}

void EngineZero::enterHitMode(int i, int j)
{
	hitModeAttack.first = i;
	hitModeAttack.second = j;
	hitModeStartingPoint = hitModeAttack;
	directionChangedCounter = 0;

	// Random between left and up so we aren't predictable
	if (getRandom(0, 1) == 0) hitMode = HitModeDirection::LEFT;
	else hitMode = HitModeDirection::UP;

	// Clear enemy ship vector
	enemyShip.clear();
}

void EngineZero::nextAttackDirection()
{
	switch (hitMode)
	{
	case EngineZero::LEFT:
		hitMode = HitModeDirection::RIGHT; break;
	case EngineZero::RIGHT:
		hitMode = HitModeDirection::UP; break;
	case EngineZero::UP:
		hitMode = HitModeDirection::DOWN; break;
	case EngineZero::DOWN:
		hitMode = HitModeDirection::LEFT; break;
	}
	directionChangedCounter++;
}

int EngineZero::getRandom(int min, int max)
{
	std::uniform_int_distribution<int> uni(min, max);
	return uni(rng);
}

bool EngineZero::isAttackValid(std::pair<int, int> attack) const
{
	return CommonUtils::BoardUtils::isAttackValid(attack, numOfRows, numOfColumns);
}

void EngineZero::moveToSecondaryAttacks(std::pair<int, int> attack)
{
	int key = CommonUtils::MapUtils::indexesToKey(numOfColumns, attack.first, attack.second);
	if (isAttackValid(attack) && attacksMap.count(key) > 0) {
		attacksMap.erase(key);
		secondaryAttacksMap[key] = attack;
	}
}

void EngineZero::removeAttack(int row, int col)
{
	int key = CommonUtils::MapUtils::indexesToKey(numOfColumns, row, col);
	attacksMap.erase(key);
	secondaryAttacksMap.erase(key);
}

bool EngineZero::isSquareInAttacksMaps(int row, int col) const
{
	int key = CommonUtils::MapUtils::indexesToKey(numOfColumns, row, col);
	return (attacksMap.count(key) > 0 || secondaryAttacksMap.count(key) > 0);
}

void EngineZero::setBoard(int player, const char** board, int numRows, int numCols)
{
	numOfRows = numRows;
	numOfColumns = numCols;
	myPlayerNum = player;

	// Copy board to boardMap and init attackVector
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {

			// Square with my ship, don't add to attack list
			if (CommonUtils::ShipUtils::isShip(board[i][j])) {
				continue;
			}
			// Empty square
			else {
				// Add it to attack map if adjacent squares empty
				if (adjacentSquaresEmpty(board, numRows, numCols, i, j)) {
					attacksMap[CommonUtils::MapUtils::indexesToKey(numCols, i + 1, j + 1)] = std::pair<int, int>(i + 1, j + 1);
				}
			}
		}
	}

	hitMode = HitModeDirection::OFF;
}

bool EngineZero::init(const std::string& path)
{
	return true;
}

std::pair<int, int> EngineZero::attack()
{
	std::pair<int, int> attack(-1, -1);

	// Hit mode
	if (hitMode != HitModeDirection::OFF) {
		switch (hitMode) {
		case HitModeDirection::LEFT:
			hitModeAttack.second--; break;
		case HitModeDirection::RIGHT:
			hitModeAttack.second++; break;
		case HitModeDirection::UP:
			hitModeAttack.first--; break;
		case HitModeDirection::DOWN:
			hitModeAttack.first++; break;
		}

		// If attack is invalid or attack not in attacks map, change direction
		if (!isAttackValid(hitModeAttack) || !isSquareInAttacksMaps(hitModeAttack.first, hitModeAttack.second)) {
			nextAttackDirection();
			hitModeAttack = hitModeStartingPoint;

			// Searched in all directions already, get out of hit mode
			if (directionChangedCounter > 4) hitMode = HitModeDirection::OFF;

			hitModeAttack = this->attack();
		}

		attack = hitModeAttack;
	}

	else if (!attacksMap.empty()) {
		// Random attack so we aren't predictable
		auto random_it = std::next(std::begin(attacksMap), getRandom(0, (int)attacksMap.size() - 1));
		attack = random_it->second;
	}

	// attacksMap is empty, move to secondary attacks
	else if (!secondaryAttacksMap.empty()) {
		attack = secondaryAttacksMap.begin()->second;
	}

	return attack;
}

void EngineZero::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	// Other player attacks
	if (player != myPlayerNum) {

		// Square is in attacks map
		if (isSquareInAttacksMaps(row, col)) {
			// They hit their own ship (has to be their ship because square is in attacks map)
			if (result == AttackResult::Hit) {
				// If not in hit mode, go into it
				if (hitMode == HitModeDirection::OFF) {
					enterHitMode(row, col);
					removeAttack(row, col);
				}

				// If in hit mode, ignore this.
			}

			// They missed. Assume they are smart and won't bother attacking
			// an obviously empty square - move all adjacent squares to secondary attacks map
			// since if they have a ship next to the attacked square they wouldn't attack this square.
			// Add to secondary attacks just in case they are stupid or misleading
			else if (result == AttackResult::Miss) {
				removeAttack(row, col);

				// Move surrounding squares
				std::pair<int, int> adjacentSquare;
				adjacentSquare.first = row + 1; adjacentSquare.second = col;
				moveToSecondaryAttacks(adjacentSquare);
				adjacentSquare.first = row - 1; adjacentSquare.second = col;
				moveToSecondaryAttacks(adjacentSquare);
				adjacentSquare.first = row; adjacentSquare.second = col + 1;
				moveToSecondaryAttacks(adjacentSquare);
				adjacentSquare.first = row; adjacentSquare.second = col - 1;
				moveToSecondaryAttacks(adjacentSquare);
			}

			// Sinked their own ship - don't bother attacking there
			else {
				removeAttack(row, col);
			}
		}
	}

	// We attacked
	else {

		// Remove attack from attacks maps
		removeAttack(row, col);

		// Missed in hit mode, change direction
		if (result == AttackResult::Miss) {
			if (hitMode != HitModeDirection::OFF) {
				hitModeAttack = hitModeStartingPoint;
				nextAttackDirection();
			}
		}

		// Hit
		else if (result == AttackResult::Hit) {
			// Enter hit mode if not already in it
			if (hitMode == HitModeDirection::OFF) {
				enterHitMode(row, col);
			}

			// Add square to enemy ship vector
			enemyShip.push_back(std::pair<int, int>(row, col));
		}

		// Sink
		else {
			// Add square to enemy ship vector
			enemyShip.push_back(std::pair<int, int>(row, col));

			// Get out of hit mode
			hitMode = HitModeDirection::OFF;

			// Remove squares around enemy ship from attacks map
			for (auto it = enemyShip.begin(); it != enemyShip.end(); it++) {
				removeAdjacentSquares(*it);
			}
		}
	}
}

IBattleshipGameAlgo* GetAlgorithm() {
	return new EngineZero();
}
