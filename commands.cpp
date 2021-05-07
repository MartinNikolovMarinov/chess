#include "commands.h"

i32 ParseMoveCommand(const std::string &_input, FieldPos &_out, std::string &_errMsg) {
	if (_input.length() != 2) {
		_errMsg = "Invalid input format. Try A1 for example.";
		return -1;
	};

	char rowChar = _input[0];
	char colChar = _input[1];
	i32 row = -1, col = -1;
	if ('1' <= rowChar && rowChar <= '9') row = ChessDigitToCanonicalPos(rowChar);
	if ('A' <= colChar && colChar <= 'H') col = ChessLetterToCanonicalPos(colChar);

	if (row == -1) {
		_errMsg = "Invalid Row.";
		return -1;
	}
	if (col == -1) {
		_errMsg = "Invalid Col.";
		return -1;
	};

	_out.Row = row;
	_out.Col = col;
	return 0;
}

i32 PromptPlayerInput(GameState &_gm, FieldPos &_from, FieldPos &_to) {
	std::cout << "Player " << _gm.currPlayer << " make a move!" <<  std::endl;
	std::cout << "Type row first then col, for example 1A is a valid location." <<  std::endl;
	std::cout << "Type the location of the peace you want to move: ";
	std::cin >> _gm.cmdInputLine;
	i32 res = ParseMoveCommand(_gm.cmdInputLine, _from, _gm.errMsg);
	if (res < 0) {
		return res;
	}

	std::cout << "Type the location you want to move it: ";
	std::cin >> _gm.cmdInputLine;
	res = ParseMoveCommand(_gm.cmdInputLine, _to, _gm.errMsg);
	if (res < 0) {
		return res;
	}

	return 0;
}