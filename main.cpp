#include "display_buf.h"
#include "chess_board.h"
#include "text.h"
#include "game_state.h"
#include "commands.h"

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

		gm.CalcOpponentAttackVect();

		// FIXME: TMP code, This should probably be below the render, but it's here because of the debug ~ render.
		gm.chessBoard.Debug_SetColorsForAttack(gm.opponentAttackVect);
		if (gm.IsCurrPlayerInCheck() == true) {
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

		// Check move validity
		std::vector<FieldPos> lMoves;
		mv.PushPieceLegalMoves(cb, gm.GetFromPiece(), gm.currMovingFrom, lMoves);
		bool found = false;
		for (i32 i = 0; i < lMoves.size(); i++) {
			FieldPos cmv = lMoves[i];
			if (cmv == gm.currMovingTo) {
				found = true;
				break;
			}
		}
		if (!found) {
			gm.errMsg = "Piece can NOT move like that";
			continue;
		}

		Piece fromPiece = gm.GetFromPiece();
		Piece toPieceCopy = Piece(gm.GetToPiece());

		// Make move:
		cb.SetPieceAt(gm.currMovingTo.Row, gm.currMovingTo.Col, fromPiece);
		cb.SetPieceAt(gm.currMovingFrom.Row, gm.currMovingFrom.Col, EMPTY_PIECE);

		// Recalculate opponent's attack vector after move:
		gm.CalcOpponentAttackVect();

		// Recalculate check validation after a move was made:
		if (gm.IsCurrPlayerInCheck() == true) {
			// Undo move:
			cb.SetPieceAt(gm.currMovingTo.Row, gm.currMovingTo.Col, toPieceCopy);
			cb.SetPieceAt(gm.currMovingFrom.Row, gm.currMovingFrom.Col, fromPiece);
			gm.errMsg = "Move is illegal, because you are in CHECK";
			continue;
		}

		// --FIXME: TMP code

		// Exact correct place for player rotation (do NOT move):
		gm.RotatePlayer();
	}
}
