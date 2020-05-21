#pragma once

#include <string>
#include <iostream>
#include <windows.h>

using namespace std;

namespace Utils {
	class ArgsParser {
		// Turns path to full path
		static string toFullPath(const string& path);
	public:
		// Returns true iff arguments are valid
		static bool parseArgs(int argc, char **argv, string *directory, bool *quiet, int *delay);
	};

	class BoardFilter {
	public:
		// Keep only player's ship on board
		static void filterBoard(const char** fullBoard, char** filteredBoard, int rows, int columns, int player);
	};
}