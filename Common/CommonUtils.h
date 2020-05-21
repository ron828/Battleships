#pragma once
#include <string>
#include <windows.h>
#include <vector>
#include <iostream>
#include <set>

using namespace std;

namespace CommonUtils
{
	class FileUtils
	{
	public:
		// Returns the Nth file's name with exension ext (in alphabetic order).
		// If not found returns empty string.
		// If there are less than N files with ext, returns empty string
		static std::string getNthFileWithExt(std::string path, int N, std::string ext);
	};

	class ShipUtils {
	public:
		// Returns true if char is ship
		static bool isShip(const char c);

		// Returns true if char c represents player A ship
		static bool isPlayerAShip(const char c);
	};

	class BoardUtils {
	public:
		// Allocates memory for a board of size rowsxcols
		static char** allocateCharBoard(const int rows, const int columns);

		// Deletes allocated memory
		static void freeCharBoard(char** board, const int rows, const int columns);

		// Allocate vector char board
		static void allocateVectorBoard(vector<char**>& board, const int rows, const int columns, const int depth);

		// Delete vector board
		static void freeVectorBoard(vector<char**>& board, const int rows, const int columns, const int depth);

		// Returns true iff attack is valid for this board size
		static bool const isAttackValid(pair<int, int>& attack, int rows, int columns);

		// Returns true iff attack is valid for this board size
		static bool const isAttackValid(int i, int j, int z, int rows, int columns, int depth);
	};

	class MapUtils {
	public:
		// Convert indexes of square to map key: [2,3] -> 23
		static int indexesToKey(int cols, int i, int j);

		// Same for 3D
		static int indexesToKey(int rows, int cols, int i, int j, int k);
	};
}