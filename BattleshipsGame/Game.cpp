#include "Game.h"

bool Game::readBoardFile(const string& boardFilePath)
{
	ifstream boardFile(boardFilePath);
	if (!boardFile.is_open()) return false;
	string line;
	for (int i = 0; i < rows; i++) {
		line = "";
		if (!boardFile.eof()) {
			getline(boardFile, line);
		}
		for (int j = 0; j < columns; j++) {
			if (j < line.length() && CommonUtils::ShipUtils::isShip(line[j])) {
				charBoard[i][j] = line[j];
			}

			// Line too short or non-ship character
			else {
				charBoard[i][j] = '-';
			}
		}
	}
	boardFile.close();
	return true;
}

IBattleshipGameAlgo * Game::getNewPlayer(int player, const string& dllFullName)
{
	// Load dynamic library
	HINSTANCE hDll = LoadLibraryA(dllFullName.c_str());
	if (!hDll)
	{
		std::cout << "Cannot load dll: " + dllFullName << std::endl;
		return nullptr;
	}
	if (player == 0) hDLL0 = hDll;
	else hDLL1 = hDll;

	// Get function pointer
	GetAlgoFuncType getAlgoFunc = (GetAlgoFuncType)GetProcAddress(hDll, "GetAlgorithm");
	if (!getAlgoFunc)
	{
		std::cout << "Cannot load dll: " + dllFullName << std::endl;
		return nullptr;
	}
	return getAlgoFunc();
}

bool Game::isGameOver() const
{
	// Both out of attacks
	if (player0Status.getIsOutOfAttacks() && player1Status.getIsOutOfAttacks()) return true;

	// A player won
	int shipsPerPlayer = board.getNumOfShips() / 2;
	if (player0Status.getSinkedShips() == shipsPerPlayer
		|| player1Status.getSinkedShips() == shipsPerPlayer)
	{
		return true;
	}

	return false;
}

AttackResult Game::doAttack(int i, int j)
{
	AttackResult result = AttackResult::Miss;

	int player = playerTurn;
	Ship* ship = board.getShipInSquare(i, j);

	if (!ship) {

		// Switch turns on miss
		playerTurn = 1 - playerTurn;
		result = AttackResult::Miss;
	}
	else {

		// Self-hit, other player gets the points
		if (player == ship->getPlayer()) {
			if (player == 0) player = 1;
			else player = 0;
		}

		// Only make changes if not already hit this square
		if (charBoard[i][j] != '@') {

			playerTurn = player;

			charBoard[i][j] = '@';
			result = AttackResult::Hit;
			int health = ship->shipHit();

			// Sinked ship
			if (health == 0) {
				result = AttackResult::Sink;

				if (player == 0) {
					player0Status.addToScore(ship->getValue());
					player0Status.shipSank();
				}
				else {
					player1Status.addToScore(ship->getValue());
					player1Status.shipSank();
				}
			}
		}

		// Already hit square
		else {

			// Switch turns if hit square a second time
			playerTurn = 1 - playerTurn;

			// Report miss if ship sunk, hit otherwise
			if (ship->getHealth() == 0) {
				result = AttackResult::Miss;
			}
			else {
				result = AttackResult::Hit;
			}
		}
	}

	return result;
}

Game::Game(bool printMoves, int moveDelay) : gameOver(false), playerTurn(0), quiet(printMoves), delay(moveDelay), player0(nullptr), player1(nullptr), hDLL0(NULL), hDLL1(NULL)
{
	charBoard = CommonUtils::BoardUtils::allocateCharBoard(rows, columns);
}

Game::~Game()
{
	CommonUtils::BoardUtils::freeCharBoard(charBoard, rows, columns);
	delete player0;
	delete player1;
	if (hDLL0) FreeLibrary(hDLL0);
	if (hDLL1) FreeLibrary(hDLL1);
}

bool Game::initGame(const std::string & path)
{
	// Get board file full name
	string boardFileFullName = CommonUtils::FileUtils::getNthFileWithExt(path, 1, "sboard");
	
	// No file found
	bool initFailed = false;
	if (boardFileFullName == "") {
		cout << "Missing board file (*.sboard) looking in path: " + path << endl;
		initFailed = true;
	}
	// Found board file, read and validate it
	else {
		// Read file into charBoard
		if (!readBoardFile(boardFileFullName)) {
			initFailed = true;
		}
		// Successfully read board
		else {
			// Validate board
			if (!BoardValidator::validateBoard(charBoard, rows, columns, shipsPerPlayer)) {
				initFailed = true;
			}
		}
	}

	// Init game board
	if (!initFailed) board.initBoard(charBoard, rows, columns);

	// Get dll files
	string dll0FileName = CommonUtils::FileUtils::getNthFileWithExt(path, 1, "dll");
	string dll1FileName = CommonUtils::FileUtils::getNthFileWithExt(path, 2, "dll");
	if (dll0FileName == "" || dll1FileName == "") {
		cout << "Missing an algorithm (dll) file looking in path: " + path << endl;
		return false;
	}

	// Found DLLs but there were previous errors
	if (initFailed) return false;

	// Load 1st DLLs
	player0 = getNewPlayer(0, dll0FileName);
	if (player0 == nullptr) return false;

	// Filter board for player 0
	char** filteredBoard = CommonUtils::BoardUtils::allocateCharBoard(rows, columns);
	Utils::BoardFilter::filterBoard((const char**)charBoard, filteredBoard, rows, columns, 0);
	player0->setBoard(0, (const char**)filteredBoard, rows, columns);

	// Init 1st player
	if (!player0->init(path)) {
		cout << "Algorithm initialization failed for dll: " + dll0FileName << endl;
		CommonUtils::BoardUtils::freeCharBoard(filteredBoard, rows, columns);
		return false;
	}

	// Load 2nd DLL
	player1 = getNewPlayer(1, dll1FileName);
	if (player1 == nullptr) return false;

	// Filter board for player 1
	Utils::BoardFilter::filterBoard((const char**)charBoard, filteredBoard, rows, columns, 1);
	player1->setBoard(1, (const char**)filteredBoard, rows, columns);

	// Init 2nd player
	if (!player1->init(path)) {
		cout << "Algorithm initialization failed for dll: " + dll1FileName << endl;
		CommonUtils::BoardUtils::freeCharBoard(filteredBoard, rows, columns);
		return false;
	}

	// Free filtered board
	CommonUtils::BoardUtils::freeCharBoard(filteredBoard, rows, columns);

	// Init players' status
	player0Status.initStatus(0);
	player1Status.initStatus(1);
	player0StatusPrev.initStatus(0);
	player1StatusPrev.initStatus(1);

	// Print board
	if (!quiet) {
		gameVisuals.initGameSize(rows, columns, shipsPerPlayer);
		gameVisuals.hideCursor(true);
		gameVisuals.printCharBoard(charBoard, rows, columns);
		gameVisuals.initStatusTable();
		cout << endl;
	}

	return true;
}

void Game::nextMove()
{
	if (isGameOver()) {
		gameOver = true;
		return;
	}

	pair<int, int> attack;
	int attacker = 0;

	// Player 0's turn
	if (playerTurn == 0) {
		attack = player0->attack();
		// Out of attacks
		if (attack.first == -1) {
			player0Status.setOutOfAttacks();
			playerTurn = 1;
			return nextMove();
		}
		attacker = 0;
	}
	// Player 1's turn
	else {
		attack = player1->attack();
		// Out of attacks
		if (attack.first == -1) {
			player1Status.setOutOfAttacks();
			playerTurn = 0;
			return nextMove();
		}
		attacker = 1;
	}

	// Invalid attack - ignorer and switch turns
	if (!CommonUtils::BoardUtils::isAttackValid(attack, rows, columns)) {
		playerTurn = 1 - playerTurn;
		return nextMove();
	}

	if (!quiet) {
		gameVisuals.printPlayerTurn(playerTurn);
	}

	AttackResult attackResult = doAttack(attack.first - 1, attack.second - 1);

	if (!quiet) {
		gameVisuals.updateBoardVisual(attacker, charBoard, board, attackResult, attack.first, attack.second, player0Status, player1Status, player0StatusPrev, player1StatusPrev);
	}

	// Notify players on result
	player0->notifyOnAttackResult(attacker, attack.first, attack.second, attackResult);
	player1->notifyOnAttackResult(attacker, attack.first, attack.second, attackResult);

	if (!quiet) {
		Sleep(delay);
	}
}

void Game::printResults()const
{
	if (!quiet) {
		gameVisuals.printGameOver();
	}

	// Check who won
	int winner = -1;
	int ships = board.getNumOfShips() / 2;
	if (player0Status.getSinkedShips() == ships) winner = 0;
	else if (player1Status.getSinkedShips() == ships) winner = 1;

	if (!quiet) {
		gameVisuals.gotoxy(0, columns + 1);
		gameVisuals.hideCursor(false);
	}
	if (winner == 0) cout << "Player A won" << endl;
	else if (winner == 1) cout << "Player B won" << endl;
	cout << "Points:" << endl;
	cout << "Player A: " + to_string(player0Status.getScore()) << endl;
	cout << "Player B: " + to_string(player1Status.getScore()) << endl;
}

bool Game::isOver()const
{
	return gameOver;
}
