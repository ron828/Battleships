#include "BoardValidator.h"
#include "../Common/CommonUtils.h"

void BoardValidator::validateShip(char ** board, int rows, int columns, int i, int j, int shipSize, map<int, int>& shipsMap, bool * shipRightSizeAndShape, bool * noAdjacentShips)
{
	char ship = board[i][j];
	int key = i*columns + j;
	shipsMap[key] = 1;
	*shipRightSizeAndShape = true;
	*noAdjacentShips = true;
	bool verticalShip = false;
	bool horizontalShip = false;

	// Check nearby squares
	visitSquaresToRight(board, rows, columns, ship, i, j, shipsMap, key, noAdjacentShips, &verticalShip, &horizontalShip);
	visitSquaresDown(board, rows, columns, ship, i, j, shipsMap, key, noAdjacentShips, &verticalShip, &horizontalShip);

	// Check if correct size
	if (shipsMap[key] != shipSize) *shipRightSizeAndShape = false;

	// Check if correct shape
	if (verticalShip && horizontalShip) *shipRightSizeAndShape = false;
}

void BoardValidator::visitSquaresToRight(char ** board, int rows, int columns, char ship, int i, int j, map<int, int>& shipsMap, int shipOriginKey, bool * noAdjacentShips, bool * verticalShip, bool * horizontalShip)
{
	for (int k = j + 1; k < columns; k++) {

		// Found ship, increment by 1
		if (board[i][k] == ship) {

			// First time visiting, update size and check ships down
			if (shipsMap.count(i*columns + k) <= 0) {
				shipsMap[shipOriginKey] = shipsMap[shipOriginKey] + 1;

				// Marks square as visited
				shipsMap[i*columns + k] = -1;

				visitSquaresDown(board, rows, columns, ship, i, k, shipsMap, shipOriginKey, noAdjacentShips, verticalShip, horizontalShip);
			}
			*horizontalShip = true;
		}

		// First time encountering non-ship character, break
		else {

			// If it's another ship, there is an adjacent ship
			if (CommonUtils::ShipUtils::isShip(board[i][k])) *noAdjacentShips = false;
			break;
		}
	}
}

void BoardValidator::visitSquaresDown(char ** board, int rows, int columns, char ship, int i, int j, map<int, int>& shipsMap, int shipOriginKey, bool * noAdjacentShips, bool * verticalShip, bool * horizontalShip)
{
	for (int k = i + 1; k < rows; k++) {

		// Found ship, increment by 1
		if (board[k][j] == ship) {

			// First time visiting, update size and check ships to right
			if (shipsMap.count(k*columns + j) <= 0) {
				shipsMap[shipOriginKey] = shipsMap[shipOriginKey] + 1;

				// Init checked square to something so we don't check it again later
				shipsMap[k*columns + j] = -1;

				visitSquaresToRight(board, rows, columns, ship, k, j, shipsMap, shipOriginKey, noAdjacentShips, verticalShip, horizontalShip);
			}

			*verticalShip = true;
		}

		// First time encountering non-ship character, break
		else {

			// If it's another ship, there is an adjacent ship
			if (CommonUtils::ShipUtils::isShip(board[k][j])) *noAdjacentShips = false;
			break;
		}
	}
}

const bool BoardValidator::validateBoard(char ** board, int rows, int columns, int shipsPerPlayer)
{
	map<int, int> shipsMap;
	map<char, bool> reportedShip;
	char c;
	string errMsg;
	char player = 'A';
	int shipSize = 0;
	int player0ShipsNum = 0;
	int player1ShipsNum = 0;
	bool shipRightSizeAndShape = true;
	bool noAdjacentShips = true;
	bool foundAdjacentShips = false;
	bool boardValid = true;

	// Make sure we don't report on same type of ship more than once
	reportedShip['b'] = reportedShip['B'] = reportedShip['p'] = reportedShip['P'] = reportedShip['m'] = reportedShip['M'] = reportedShip['d'] = reportedShip['D'] = false;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {

			// Already processed this square, skip it
			if (shipsMap.count(i*columns + j) > 0) continue;

			c = board[i][j];

			// If not a ship, continue
			if (!CommonUtils::ShipUtils::isShip(c)) continue;

			switch (c) {
			case 'B':
				shipSize = 1;
				player = 'A';
				break;
			case 'b':
				shipSize = 1;
				player = 'B';
				break;
			case 'P':
				shipSize = 2;
				player = 'A';
				break;
			case 'p':
				shipSize = 2;
				player = 'B';
				break;
			case 'M':
				shipSize = 3;
				player = 'A';
				break;
			case 'm':
				shipSize = 3;
				player = 'B';
				break;
			case 'D':
				shipSize = 4;
				player = 'A';
				break;
			case 'd':
				shipSize = 4;
				player = 'B';
				break;
			}

			validateShip(board, rows, columns, i, j, shipSize, shipsMap, &shipRightSizeAndShape, &noAdjacentShips);

			if (!shipRightSizeAndShape) {
				boardValid = false;
				if (!reportedShip[c]) {
					errMsg = "Wrong size or shape for ship ";
					errMsg.push_back(c);
					errMsg += " for player ";
					errMsg.push_back(player);
					cout << errMsg << endl;
				}
				reportedShip[c] = true;
			}

			// Valid ship, add to player's ship counter
			else {
				if (player == 'A') player0ShipsNum++;
				else player1ShipsNum++;
			}
			if (!noAdjacentShips) {
				foundAdjacentShips = true;
			}
		}
	}

	// Too many/few ships
	if (player0ShipsNum > shipsPerPlayer) {
		cout << "Too many ships for player A" << endl;
		boardValid = false;
	}
	if (player1ShipsNum > shipsPerPlayer) {
		cout << "Too many ships for player B" << endl;
		boardValid = false;
	}
	if (player0ShipsNum < shipsPerPlayer) {
		cout << "Too few ships for player A" << endl;
		boardValid = false;
	}
	if (player1ShipsNum < shipsPerPlayer) {
		cout << "Too few ships for player B" << endl;
		boardValid = false;
	}

	// Last message - adjacent ships on board
	if (foundAdjacentShips) {
		boardValid = false;
		cout << "Adjacent Ships on Board" << endl;
	}
	return boardValid;
}
