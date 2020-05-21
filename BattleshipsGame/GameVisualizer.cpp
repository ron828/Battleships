#include "GameVisualizer.h"

void GameVisualizer::setPlayerColor(int player)const
{
	static HANDLE h = NULL;
	if (!h) h = GetStdHandle(STD_OUTPUT_HANDLE);
	if (player == 0) SetConsoleTextAttribute(h, 0x0009);
	else SetConsoleTextAttribute(h, 0x000E);
}

void GameVisualizer::restoreColor()const
{
	static HANDLE h = NULL;
	if (!h) h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, 0x0007);
}

void GameVisualizer::updateSquareVisual(int x, int y, char c)const
{
	for (int i = 0; i < 4; i++) {
		gotoxy(x, y);
		cout << " ";
		Sleep(100);
		gotoxy(x, y);
		cout << c;
		Sleep(50);
	}
}

int GameVisualizer::keyToColumn(int key)const
{
	return key % columns;
}

int GameVisualizer::keyToRow(int key)const
{
	return key / columns;
}

void GameVisualizer::initGameSize(int rowsNum, int columnsNum, int shipsPerPlayerNum)
{
	rows = rowsNum;
	columns = columnsNum;
	shipsPerPlayer = shipsPerPlayerNum;
}

void GameVisualizer::printCharBoard(char ** board, int rows, int columns)const
{
	system("cls");
	static HANDLE h = NULL;
	if (!h) h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, FOREGROUND_GREEN);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			if (CommonUtils::ShipUtils::isShip(board[i][j])) {
				if (CommonUtils::ShipUtils::isPlayerAShip(board[i][j])) SetConsoleTextAttribute(h, 0x0009);
				else SetConsoleTextAttribute(h, 0x000E);
			}
			else {
				SetConsoleTextAttribute(h, 0x0008);
			}
			cout << board[i][j];
			cout << " ";
		}
		cout << endl;
	}
	SetConsoleTextAttribute(h, 0x0008);
}

void GameVisualizer::gotoxy(int x, int y)const
{
	static HANDLE h = NULL;
	if (!h) h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { (short)x, (short)y };
	SetConsoleCursorPosition(h, c);
}

void GameVisualizer::updateBoardVisual(int player, char ** charBoard, Board & board, AttackResult result, int row, int column, PlayerStatus & player0Status, PlayerStatus & player1Status, PlayerStatus & player0StatusPrev, PlayerStatus & player1StatusPrev)const
{
	int x = column - 1;
	int y = row - 1;

	// Update game status
	gotoxy(columns * 2 + 2, 1);
	slowPrint("Attacking (" + to_string(row) + "," + to_string(column) + ")...");

	// Mark attack on board
	gotoxy(x * 2, y);
	if (result == AttackResult::Hit) {
		Ship* ship = board.getShipInSquare(row - 1, column - 1);
		setPlayerColor(ship->getPlayer());
		updateSquareVisual(x * 2, y, '*');
	}
	else if (result == AttackResult::Miss) {
		static HANDLE h = NULL;
		if (!h) h = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(h, 0x000C);
		updateSquareVisual(x * 2, y, 'X');
	}
	else if (result == AttackResult::Sink) {
		Ship* ship = board.getShipInSquare(row - 1, column - 1);
		setPlayerColor(ship->getPlayer());
		updateSquareVisual(x * 2, y, '*');
		for (int i = 0; i < ship->getSize(); i++) {
			int a = keyToColumn(ship->getSquare(i));
			int b = keyToRow(ship->getSquare(i));
			updateSquareVisual(a * 2, b, '@');
		}
	}
	restoreColor();

	// Print result
	string resultString = "";
	if (result == AttackResult::Hit) resultString = "Hit!";
	else if (result == AttackResult::Miss) resultString = "Miss!";
	else if (result == AttackResult::Sink) resultString = "Sink!";
	gotoxy(columns * 2 + 2, 2);
	slowPrint(resultString);
	if (result == AttackResult::Miss) {
		gotoxy(x * 2, y);
		char c = charBoard[row - 1][column - 1];
		if (c != '-') { // Sunk ship returns miss, restore '@' with proper color
			Ship* ship = board.getShipInSquare(row - 1, column - 1);
			setPlayerColor(ship->getPlayer());
		}
		else {
			static HANDLE h = NULL;
			if (!h) h = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(h, 0x000C);
		}
		cout << c;
		restoreColor();
	}
	else if (result == AttackResult::Sink) {
		updateStatusTable(player0Status, player1Status, player0StatusPrev, player1StatusPrev);
	}
}

void GameVisualizer::slowPrint(string str)const
{
	for (int i = 0; i < str.length(); i++) {
		cout << str[i];
		Sleep(10);
	}
}

void GameVisualizer::initStatusTable()const
{
	GameVisualizer::restoreColor();
	gotoxy(columns * 2 + 2, 3);
	cout << "+--------+--------+-------+";
	gotoxy(columns * 2 + 2, 4);
	cout << "| Player | Points | Ships |";
	gotoxy(columns * 2 + 2, 5);
	cout << "+--------+--------+-------+";
	gotoxy(columns * 2 + 2, 6);
	cout << "|   ";
	setPlayerColor(0);
	cout << "A";
	restoreColor();
	cout << "    |   0    |   " << to_string(shipsPerPlayer) << "   |";
	gotoxy(columns * 2 + 2, 7);
	cout << "+--------+--------+-------+";
	gotoxy(columns * 2 + 2, 8);
	cout << "|   ";
	setPlayerColor(1);
	cout << "B";
	restoreColor();
	cout << "    |   0    |   " << to_string(shipsPerPlayer) << "   |";
	gotoxy(columns * 2 + 2, 9);
	cout << "+--------+--------+-------+";
}

void GameVisualizer::updateStatusTable(PlayerStatus & player0Status, PlayerStatus & player1Status, PlayerStatus & player0StatusPrev, PlayerStatus & player1StatusPrev)const
{
	// Score
	int player0ScoreDiff = player0Status.getScore() - player0StatusPrev.getScore();
	if (player0ScoreDiff > 0) {
		// Show difference visual (+#)
		gotoxy(columns * 2 + 2 + 13 + 2, 6);
		setPlayerColor(0);
		cout << "+";
		updateSquareVisual(columns * 2 + 2 + 13 + 2 + 1, 6, '0' + player0ScoreDiff);
		restoreColor();
		gotoxy(columns * 2 + 2 + 13 + 2, 6);
		cout << "  ";

		gotoxy(columns * 2 + 2 + 13, 6);
		cout << to_string(player0Status.getScore());

		player0StatusPrev.addToScore(player0ScoreDiff);
	}

	int player1ScoreDiff = player1Status.getScore() - player1StatusPrev.getScore();
	if (player1ScoreDiff > 0) {

		// Show difference visual (+#)
		gotoxy(columns * 2 + 2 + 13 + 2, 8);
		setPlayerColor(1);
		cout << "+";
		updateSquareVisual(columns * 2 + 2 + 13 + 2 + 1, 8, '0' + player1ScoreDiff);
		restoreColor();
		gotoxy(columns * 2 + 2 + 13 + 2, 8);
		cout << "  ";

		gotoxy(columns * 2 + 2 + 13, 8);
		cout << to_string(player1Status.getScore());

		player1StatusPrev.addToScore(player1ScoreDiff);
	}

	// Ships
	int player0ShipsDiff = player1Status.getSinkedShips() - player1StatusPrev.getSinkedShips();
	if (player0ShipsDiff > 0) {
		// Show difference visual (-1)
		gotoxy(columns * 2 + 2 + 22 + 2, 6);
		setPlayerColor(0);
		cout << "-";
		updateSquareVisual(columns * 2 + 2 + 22 + 2 + 1, 6, '1');
		restoreColor();
		gotoxy(columns * 2 + 2 + 22 + 2, 6);
		cout << "  ";

		gotoxy(columns * 2 + 2 + 22, 6);
		cout << to_string(shipsPerPlayer - player1Status.getSinkedShips());

		player1StatusPrev.shipSank();
	}

	int player1ShipsDiff = player0Status.getSinkedShips() - player0StatusPrev.getSinkedShips();
	if (player1ShipsDiff > 0) {
		// Show difference visual (-1)
		gotoxy(columns * 2 + 2 + 22 + 2, 8);
		setPlayerColor(1);
		cout << "-";
		updateSquareVisual(columns * 2 + 2 + 22 + 2 + 1, 8, '1');
		restoreColor();
		gotoxy(columns * 2 + 2 + 22 + 2, 8);
		cout << "  ";

		gotoxy(columns * 2 + 2 + 22, 8);
		cout << to_string(shipsPerPlayer - player0Status.getSinkedShips());

		player0StatusPrev.shipSank();
	}
}

void GameVisualizer::printPlayerTurn(int player)const
{
	// Delete previous status
	for (int i = 0; i < 3; i++) {
		gotoxy(columns * 2 + 2, i);
		cout << "                                    ";
	}
	gotoxy(columns * 2 + 2, 0);
	string playerName = "A";
	if (player == 1) playerName = "B";
	slowPrint("Player " + playerName + " playing...");
}

void GameVisualizer::printGameOver()const
{
	for (int i = 0; i < 3; i++) {
		gotoxy(columns * 2 + 2, i);
		cout << "                                    ";
	}
	gotoxy(columns * 2 + 2, 0);
	cout << "Game Over.";
}

void GameVisualizer::hideCursor(bool hide)const
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	if (hide) cursorInfo.bVisible = false;
	else cursorInfo.bVisible = true;
	SetConsoleCursorInfo(out, &cursorInfo);
}
