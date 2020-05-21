#include "Utils.h"

string Utils::ArgsParser::toFullPath(const string & path)
{
	char full[_MAX_PATH];
	if (_fullpath(full, path.c_str(), _MAX_PATH) != NULL) {
		return full;
	}
	return "";
}

bool Utils::ArgsParser::parseArgs(int argc, char ** argv, string * directory, bool * quiet, int * delay)
{
	if (argc > 1) {

		// argv[1] can also be '-quiet' or '-delay'
		if (strcmp(argv[1], "-quiet") == 0) {
			*quiet = true;
		}
		else if (strcmp(argv[1], "-delay") == 0) {
			// Next argument has to be delay time
			if (argc < 3) {
				cout << "Specify delay time in ms." << endl;
				return false;
			}
			else {
				try {
					*delay = stoi(argv[2]);
				}
				catch (const std::exception&) {
					cout << "Specify delay time in ms." << endl;
					return false;
				}
				if (*delay < 0) *delay = 0;
				else if (*delay > 10000) *delay = 10000;
			}

			if (argc > 3) {
				if (strcmp(argv[3], "-quiet") == 0) {
					*quiet = true;
				}
			}
		}

		// Must be directory
		else {
			*directory = toFullPath(argv[1]);

			// Make sure path exists
			struct stat buffer;
			if (!(stat(directory->c_str(), &buffer) == 0)) {
				cout << "Wrong path: " + *directory << endl;
				return false;
			}

			if (argc > 2) {
				if (strcmp(argv[2], "-quiet") == 0) {
					*quiet = true;
				}
				else if (strcmp(argv[2], "-delay") == 0) {
					// Next argument has to be delay time
					if (argc < 3) {
						cout << "Specify delay time in ms." << endl;
						return false;
					}
					else {
						*delay = stoi(argv[3]);
						if (*delay < 0) *delay = 0;
						else if (*delay > 10000) *delay = 10000;
					}

					if (argc > 4) {
						if (strcmp(argv[4], "-quiet") == 0) {
							*quiet = true;
						}
					}
				}
			}
		}
	}

	// If directory is ".", replace it with working directory
	if (*directory == ".") {
		char currentPath[MAX_PATH];
		if (GetCurrentDirectoryA(MAX_PATH, currentPath) == 0) {
			*directory = "Working Directory";
		}
		*directory = currentPath;
	}

	return true;
}

void Utils::BoardFilter::filterBoard(const char ** fullBoard, char ** filteredBoard, int rows, int columns, int player)
{
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			filteredBoard[i][j] = fullBoard[i][j];
			if (player == 0) {
				if (fullBoard[i][j] == 'b' || fullBoard[i][j] == 'p'
					|| fullBoard[i][j] == 'm' || fullBoard[i][j] == 'd')
				{
					filteredBoard[i][j] = '-';
				}
			}
			else {
				if (fullBoard[i][j] == 'B' || fullBoard[i][j] == 'P'
					|| fullBoard[i][j] == 'M' || fullBoard[i][j] == 'D')
				{
					filteredBoard[i][j] = '-';
				}
			}
		}
	}
}
