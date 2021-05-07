#include "commands.h"

i32 parseMoveCommand(const std::string &_input, FieldPos &_out, std::string &_errMsg);

MoveCmd::MoveCmd(std::ostream &_out, std::istream &_in) : out(_out), in(_in), userInput("") {}
MoveCmd::~MoveCmd() {}

std::string& MoveCmd::GetUserInput() { return userInput; }

i32 MoveCmd::PromptPlayerInput(u32 _pid, FieldPos &_from, FieldPos &_to, std::string &_errMsg) {
	this->out << "Player " << _pid << " make a move!" <<  std::endl;
	this->out << "Type row first then col, for example 1A is a valid location." <<  std::endl;
	this->out << "Type the location of the peace you want to move: ";

	std::string inputLine;
	this->in >> inputLine;
	this->userInput += inputLine;
	i32 res = parseMoveCommand(inputLine, _from, _errMsg);
	if (res < 0) {
		return res;
	}

	this->out << "Type the location you want to move it: ";
	this->in >> inputLine;
	this->userInput += " " + inputLine;
	res = parseMoveCommand(inputLine, _to, _errMsg);
	if (res < 0) {
		return res;
	}

	return 0;
}

i32 parseMoveCommand(const std::string &_input, FieldPos &_out, std::string &_errMsg) {
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