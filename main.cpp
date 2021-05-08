#include "display_buf.h"
#include "chess_board.h"
#include "text.h"
#include "game_state.h"
#include "commands.h"

// TODO: enpassant take rule.
// TODO: add all unnecessary getters and setters to comply to OOP nonsense.

#include "move_rules.h"

int main() {
	DisplayBuffer dbuf = DisplayBuffer(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	ChessBoard cb;
	MovementRules mv;
	GameState gm = GameState(&cb, &mv);
	TextDisplay mfu = TextDisplay();
	u32 winner = 0;

	// FIXME: TMP code
	// std::stringstream debugIn(
	// 	"4E 5F\n" // not legal
	// 	"4E 5E\n"
	// 	"7H 7E\n" // mate
	// );
	// gm.RotatePlayer();
	// --FIXME: TMP code

	while(!gm.isGameOver) {
		// FIXME: TMP code
		// std::cin.get();
		// --FIXME: TMP code

		// Clean all state:
		ClearScreen();
		dbuf.Clear(' ');
		mfu.Clear();
		gm.Init();

		if (gm.errMsg.length() > 0) {
			// Prev input from user had errors.
			gm.errMsg += "\nPress enter to continue.";
			mfu.SetMsg(gm.errMsg);
			mfu.Display(&dbuf, 15, 30);
			dbuf.FlushTo(std::cout);
			gm.errMsg.clear();

			std::cin.get(); // For some reason I need cin.get twice...
			while (std::cin.get() != '\n'); // wait for user to press space and ignore any other pressed key.
			continue;
		}

		// Render to screen:
		cb.Display(&dbuf, 2, DISPLAY_CHESS_CENTER_LEFT);
		dbuf.FlushTo(std::cout);

		// Validate Checkmate state:
		if (gm.IsCurrPlayerInCheck() == true) {
			gm.isInCheck = true;
			if (gm.IsCheckmate()) {
				// opponent won
				winner = gm.GetOpponentPlayer();
				break;
			}
		}

		gm.currMovingFrom = {};
		gm.currMovingTo = {};
#if 0
		// FIXME: TMP code
		MoveCmd cmd = MoveCmd(std::cout, debugIn);
		// if (cmd.GetUserInput() == "2D 1E") {
		// 	std::cout << "\0";
		// 	// Render to screen:
		// 	cb.Display(dbuf, 2, DISPLAY_CHESS_CENTER_LEFT);
		// 	dbuf.FlushTo(std::cout);
		// }
		// std::cout << cmd.GetUserInput() << " count=" << count++;
		// --FIXME: TMP code
#else
		MoveCmd cmd = MoveCmd(std::cout, std::cin);
#endif

		if (gm.isInCheck) {
			std::cout << "You are in CHECK!" << std::endl;
		}

		// Prompt the player for input. This blocks the thread
		// so drawing needs to happen above this line!
		if (cmd.PromptPlayerInput(gm.GetCurrPlayer(), gm.currMovingFrom, gm.currMovingTo, gm.errMsg) < 0) {
			continue;
		}

		// Check move validity:
		if (gm.CheckBasicRules() == false) {
			continue;
		}
		if (gm.IsCurrMoveLegal() == false) {
			continue;
		}
		// Make move:
		if (gm.TryMakeMove() == false) {
			continue;
		}

		// After successful move rotate the current player:
		gm.RotatePlayer();
	}

	// ClearScreen();
	std::cout << "Checkmate! Player " << winner << " is the winner." << std::endl;
}
