#include "display_buf.h"
#include "chess_board.h"
#include "text.h"
#include "game_state.h"
#include "commands.h"

// TODO: all the code should be in a common namespace. (probably)
// TODO: add all unnecessary getters and setters to comply to OOP nonsense.

i32 CheckBasicRules(GameState &_gm) {
	if (_gm.GetFromPiece().IsEmpty()) {
		_gm.errMsg = "There is not piece at that location.";
		return -1;
	}

	if (_gm.currPlayer != _gm.GetFromPiece().GetPlayerId()) {
		_gm.errMsg = "This piece does NOT belong to you.";
		return -1;
	}

	if (_gm.currPlayer == _gm.GetToPiece().GetPlayerId()) {
		_gm.errMsg = "You can NOT take your own piece.";
		return -1;
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

			std::cin.get(); // TODO: first call to get reads the previous enter, for some reason.. can this be fixed?
			while (std::cin.get() != '\n'); // wait for user to press space and ignore any other pressed key.
			continue;
		}

		cb.Display(dbuf, 2, DISPLAY_CHESS_CENTER_LEFT);
		dbuf.FlushTo(std::cout);

		// Prompt the player for input. This blocks the thread
		// so drawing needs to happen above this line!
		gm.from = {};
		gm.to = {};
		if (PromptPlayerInput(gm, gm.from, gm.to) < 0) {
			continue;
		}

		// Check move validity:
		if (CheckBasicRules(gm) < 0) {
			continue;
		}

		// Move the piece TMP DEBUG code:
		const Piece fromP = gm.GetFromPiece();
		// const Piece toP = gm.GetToPiece();
		cb.field[gm.to.Row][gm.to.Col].SetPiece(fromP);
		cb.field[gm.from.Row][gm.from.Col].SetPiece(EMPTY_PIECE);

		gm.RotatePlayer();
	}
}
