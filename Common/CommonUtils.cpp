#include "CommonUtils.h"

std::string CommonUtils::FileUtils::getNthFileWithExt(std::string path, int N, std::string ext)
{
	std::set<string> filesNamesSet;
	HANDLE dir;
	WIN32_FIND_DATAA fileData;
	string fullFileName = "";

	string s = "\\*." + ext;
	dir = FindFirstFileA((path + s).c_str(), &fileData);
	if (dir != INVALID_HANDLE_VALUE)
	{
		do
		{
			filesNamesSet.insert(fileData.cFileName);

		} while (FindNextFileA(dir, &fileData));
	}

	int counter = 1;
	for (auto itr = filesNamesSet.begin(); itr != filesNamesSet.end(); itr++) {
		if (counter++ == N) {
			fullFileName = path + "\\" + *itr;
			break;
		}
	}

	return fullFileName;
}

bool CommonUtils::ShipUtils::isShip(const char c)
{
	return (c == 'B' || c == 'b' || c == 'P' || c == 'p' || c == 'M' || c == 'm' || c == 'D' || c == 'd');
}

bool CommonUtils::ShipUtils::isPlayerAShip(const char c)
{
	return c < 97;
}

char** CommonUtils::BoardUtils::allocateCharBoard(const int rows, const int columns)
{
	char** board = new char* [rows];
	for (int i = 0; i < rows; i++) {
		board[i] = new char[columns];
	}
	return board;
}

void CommonUtils::BoardUtils::freeCharBoard(char** board, const int rows, const int columns)
{
	for (int i = 0; i < rows; i++) {
		delete[] board[i];
	}
	delete[] board;
}

void CommonUtils::BoardUtils::allocateVectorBoard(vector<char**>& board, const int rows, const int columns, const int depth)
{
	for (int i = 0; i < depth; i++) {
		board.push_back(allocateCharBoard(rows, columns));
	}
}

void CommonUtils::BoardUtils::freeVectorBoard(vector<char**>& board, const int rows, const int columns, const int depth)
{
	for (int i = 0; i < depth; i++) {
		freeCharBoard(board[i], rows, columns);
	}
}

bool const CommonUtils::BoardUtils::isAttackValid(pair<int, int>& attack, int rows, int columns)
{
	return (attack.first >= 1 && attack.first <= rows && attack.second >= 1 && attack.second <= columns);
}

bool const CommonUtils::BoardUtils::isAttackValid(int i, int j, int z, int rows, int columns, int depth)
{
	return (i >= 1 && i <= rows && j >= 1 && j <= columns && z >= 1 && z <= depth);
}

int CommonUtils::MapUtils::indexesToKey(int cols, int i, int j)
{
	return i * cols + j;
}

int CommonUtils::MapUtils::indexesToKey(int rows, int cols, int i, int j, int k)
{
	return i * cols + j + k * rows * cols;
}
