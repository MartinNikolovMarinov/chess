#include "display_buf.h"
#include "chess_board.h"

// TODO: all the code should be in a common namespace. (probably)

class Command {
public:
	Piece piece;
	u32 fromRow, fromCol, toRow, toCol;

	Command();
	~Command();
};

Command::Command() {}
Command::~Command() {}

struct Pos { u32 Row; u32 Col; };

i32 ParseMoveCommand(std::string _input, Pos *out) {
	if (_input.length() != 2) return -1;

	char rowChar = _input[0];
	char colChar = _input[1];
	i32 row = -1, col = -1;
	if ('1' <= rowChar && rowChar <= '9') row = ChessDigitToCanonicalPos(rowChar);
	if ('A' <= colChar && colChar <= 'H') col = ChessLetterToCanonicalPos(colChar);

	if (row == -1) return -2;
	if (col == -1) return -3;

	out->Row = (u32)row;
	out->Col = (u32)col;
	return 0;
}


int main() {
	DisplayBuffer dbuf = DisplayBuffer(DISPLAY_WIDTH, DISPLAY_HEIGHT);

	std::string inLine;
	u32 currPlayer = 1;
	bool done = false;
	while(!done) {
		ClearScreen();
		dbuf.Clear(' ');

		ChessBoard cb;
		cb.Display(&dbuf, 2, (DISPLAY_WIDTH - CHESSBOARD_WIDTH) / 2);
		dbuf.FlushTo(std::cout);

		return 0;

		inLine.clear();
		std::cout << "Player " << currPlayer << " make a move!" <<  std::endl;

		std::cout << "Type the location of the peace you want to move: ";
		// std::cin >> inLine;
		inLine = "0A";
		Pos out = {};
		i32 res = ParseMoveCommand(inLine, &out);
		if (res < 0) {
			std::cout << "Invalid Command" << std::endl;
			continue;
		}
		std::cout << out.Row << " " << out.Col << std::endl;

		std::cout << std::endl;
		std::cout << "Type the location you want to move it: ";
		// std::cin >> inLine;
		inLine = "3C";
		out = {};
		res = ParseMoveCommand(inLine, &out);
		if (res < 0) {
			std::cout << "Invalid Command" << std::endl;
			continue;
		}
		std::cout << out.Row << " " << out.Col << std::endl;

		currPlayer = currPlayer == 1 ? 2 : 1;
	}
}
