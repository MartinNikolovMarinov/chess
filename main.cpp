#include "display_buf.h"
#include "chess_board.h"
#include "text.h"
#include "game_state.h"

// TODO: all the code should be in a common namespace. (probably)
// TODO: display function is common for all visible/window elements.

struct Command {
	Piece p;
	u32 fromRow, fromCol, toRow, toCol;
};

struct CmdPos {
	u32 row;
	u32 col;
};

i32 ParseMoveCommand(const std::string& _input, CmdPos *_out, std::string& _errMsg) {
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

	_out->row = (u32)row;
	_out->col = (u32)col;
	return 0;
}

i32 PromptPlayerInput(GameState &gm, CmdPos &from, CmdPos &to) {
	std::cout << "Player " << gm.currPlayer << " make a move!" <<  std::endl;
	std::cout << "Type row first then col, for example 1A is a valid location." <<  std::endl;
	std::cout << "Type the location of the peace you want to move: ";
	std::cin >> gm.inputLine;
	i32 res = ParseMoveCommand(gm.inputLine, &from, gm.errMsg);
	if (res < 0) {
		return res;
	}

	std::cout << std::endl;
	std::cout << "Type the location you want to move it: ";
	std::cin >> gm.inputLine;
	res = ParseMoveCommand(gm.inputLine, &to, gm.errMsg);
	if (res < 0) {
		return res;
	}

	return 0;
}

int main() {
    DisplayBuffer dbuf = DisplayBuffer(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	ChessBoard cb;
	GameState gm = GameState(cb);
	TextDisplay mfu = TextDisplay();

	char c = '1';
	while(!gm.isDone) {
		// Clean all state:
		ClearScreen();
		dbuf.Clear(' ');
		gm.Clear();
		mfu.Clear();

		if (gm.errMsg.length() > 0) {
			// Prev input from user had errors.
			gm.errMsg += "\nPress enter to continue.";
			mfu.SetMsg(gm.errMsg);
			mfu.Display(dbuf, 15, 30);
			dbuf.FlushTo(std::cout);
			gm.errMsg.clear();

			std::cin.get(); // TODO: first get reads the previous enter, for some reason.. can this be fixed?
			while (std::cin.get() != '\n'); // wait for user to press space and ignore any other pressed key.
			continue;
		}

		cb.Display(dbuf, 2, DISPLAY_CHESS_CENTER_LEFT);
		dbuf.FlushTo(std::cout);

		// Prompt the player for input. This blocks the thread
		// so drawing needs to happen above this line!
		CmdPos from = {};
		CmdPos to = {};
		if (PromptPlayerInput(gm, from, to) < 0) {
			continue;
		}

		// Move the piece TMP DEBUG code:
		const Piece fromP = cb.field[from.row][from.col].GetPiece();
		const Piece toP = cb.field[to.row][to.col].GetPiece();
		cb.field[to.row][to.col].SetPiece(&fromP);
		cb.field[from.row][from.col].SetPiece(&toP);

		gm.RotatePlayer();
	}
}
