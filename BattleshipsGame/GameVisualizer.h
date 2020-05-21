#pragma once

#include <string>
#include <iostream>
#include <windows.h>
#include "Board.h"
#include "PlayerStatus.h"
#include "IBattleshipGameAlgo.h"
#include "../Common/CommonUtils.h"

using namespace std;

class GameVisualizer {
	int rows, columns, shipsPerPlayer;

	void setPlayerColor(int player)const;
	void restoreColor()const;
	void updateSquareVisual(int x, int y, char c)const;
	int keyToColumn(int key)const;
	int keyToRow(int key)const;
public:
	GameVisualizer() : rows(0), columns(0), shipsPerPlayer(0) {};
	~GameVisualizer() = default;
	void initGameSize(int rowsNum, int columnsNum, int shipsPerPlayer);
	void printCharBoard(char ** board, int rows, int columns)const;
	void gotoxy(int x, int y)const;
	void updateBoardVisual(int player, char** charBoard, Board& board, AttackResult result, int row, int column, PlayerStatus& player0Status, PlayerStatus& player1Status, PlayerStatus& player0StatusPrev, PlayerStatus& player1StatusPrev)const;
	void slowPrint(string str)const;
	void initStatusTable()const;
	void updateStatusTable(PlayerStatus& player0Status, PlayerStatus& player1Status, PlayerStatus& player0StatusPrev, PlayerStatus& player1StatusPrev)const;
	void printPlayerTurn(int player)const;
	void printGameOver()const;
	void hideCursor(bool hide)const;
};