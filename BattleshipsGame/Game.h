#pragma once
#include <string>
#include <fstream>
#include <map>
#include "../Common/CommonUtils.h"
#include "BoardValidator.h"
#include "Board.h"
#include "IBattleshipGameAlgo.h"
#include "PlayerStatus.h"
#include "GameVisualizer.h"
#include "Utils.h"

using namespace std;

class Game {
	static const int rows = 20;
	static const int columns = 26;
	static const int shipsPerPlayer = 9;
	char** charBoard;
	Board	board;
	IBattleshipGameAlgo *player0;
	IBattleshipGameAlgo *player1;
	HINSTANCE hDLL0, hDLL1;
	PlayerStatus player0Status;
	PlayerStatus player1Status;
	PlayerStatus player0StatusPrev;
	PlayerStatus player1StatusPrev;
	GameVisualizer gameVisuals;
	int playerTurn;
	bool gameOver;
	bool quiet; // Whether to print game visuals
	int delay; // If printing, delay between turns

	// define function of the type we expect
	typedef IBattleshipGameAlgo *(*GetAlgoFuncType)();

	// Read board file into charBoard. Returns false on failure
	bool readBoardFile(const string& boardFilePath);

	// Returns pointer to new instance of player
	IBattleshipGameAlgo* getNewPlayer(int player, const string& dllFullName);

	// Return true if game is over
	bool isGameOver()const;

	// Attack square [i,j]
	// Sets playerTurn according to result.
	AttackResult doAttack(int i, int j);
	
public:
	Game(bool printMoves, int moveDelay);
	~Game();

	// Read and validate board file, load players. Returns false if fails
	// (path invalid, missing board file, invalid board etc.)
	bool initGame(const std::string& path);

	// Returns true iff game is over
	bool isOver()const;

	// Play next move
	void nextMove();

	// Print game results
	void printResults()const;
};