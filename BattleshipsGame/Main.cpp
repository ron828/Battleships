#include <iostream>
#include <string>
#include <sys/stat.h>
#include <windows.h>
#include "Game.h"
#include "Utils.h"
#include "../Common/CommonUtils.h"

using namespace std;

int main(int argc, char **argv) {
	bool quiet = false;
	int delay = 1000;
	string path = ".";

	if (!Utils::ArgsParser::parseArgs(argc, argv, &path, &quiet, &delay)) {
		return -1;
	}

	// Create new game
	Game game(quiet, delay);
	if (!game.initGame(path)) {
		return -1;
	}
	
	// Play
	while (!game.isOver()) {
		game.nextMove();
	}

	// Print results
	game.printResults();
}