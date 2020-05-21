#pragma once
#include <map>
#include <iostream>

using namespace std;

class BoardValidator {

	// Make sure ship is correct size and shape and that there are no adjacent ships.
	// Returns results in shipRightSizeAndShape and noAdjacentShips
	static void validateShip(char ** board, int rows, int columns, int i, int j, int shipSize, map<int, int>& shipsMap, bool *shipRightSizeAndShape, bool* noAdjacentShips);

	// Go right on the board until the first non-ship square.
	// Mark all ship squares as visited while updating the original ship's counter (shipOriginKey).
	// If the first non-ship square is a different ship, set noAdjacentShips to false.
	static void visitSquaresToRight(char** board, int rows, int columns, char ship, int i, int j, map<int, int>& shipsMap, int shipOriginKey, bool *noAdjacentShips, bool *verticalShip, bool* horizontalShip);

	// Same as visitSquaresToRight but go down
	static void visitSquaresDown(char ** board, int rows, int columns, char ship, int i, int j, map<int, int>& shipsMap, int shipOriginKey, bool *noAdjacentShips, bool *verticalShip, bool* horizontalShip);

public:

	// Returns true iff board is valid
	static const bool validateBoard(char** board, int rows, int columns, int shipsPerPlayer);
};