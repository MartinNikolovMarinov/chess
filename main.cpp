#include "display_buf.h"
#include "chess_board.h"
#include "text.h"
#include "game_state.h"
#include "commands.h"

// TODO: implement move vector calcuations.
// TODO: implement "game is over" logic.

// TODO: all the code should be in a common namespace. (probably)
// TODO: should use map instead of a vector for attack positions!
// TODO: add all unnecessary getters and setters to comply to OOP nonsense.

#include "move_rules.h"

int main() {
	DisplayBuffer dbuf = DisplayBuffer(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	ChessBoard cb;
	MovementRules mv;
	GameState gm = GameState(cb, mv);
	TextDisplay mfu = TextDisplay();

	while(!gm.isGameOver) {
		// Clean all state:
		ClearScreen();
		dbuf.Clear(' ');
		gm.Clear();
		mfu.Clear();

		// FIXME: TMP code
		cb.Debug_RemoveDebugColorsFromBoard();
		// --FIXME: TMP code

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

		// FIXME: TMP code, This should probably be below the render, but it's here because of the debug ~ render.
		gm.CalcOpponentAttackVect();
		if (gm.IsCurrPlayerInCheck()) {
			std::cout<<"You are in check"<<std::endl;
		}
		// --FIXME: TMP code

		// Render to screen:
		cb.Display(dbuf, 2, DISPLAY_CHESS_CENTER_LEFT);
		dbuf.FlushTo(std::cout);

		// Prompt the player for input. This blocks the thread
		// so drawing needs to happen above this line!
		gm.currMovingFrom = {};
		gm.currMovingTo = {};
		if (PromptPlayerInput(gm, gm.currMovingFrom, gm.currMovingTo) < 0) {
			continue;
		}

		// Check move validity:
		if (gm.CheckBasicRules() < 0) {
			continue;
		}

		// FIXME: TMP code
		Piece fromP = gm.GetFromPiece();
		cb.SetPieceAt(gm.currMovingTo.Row, gm.currMovingTo.Col, fromP);
		cb.SetPieceAt(gm.currMovingFrom.Row, gm.currMovingFrom.Col, EMPTY_PIECE);
		// --FIXME: TMP code

		gm.RotatePlayer();
	}
}
