#include "display_buf.h"
#include "chess_board.h"
#include "text.h"
#include "game_state.h"
#include "commands.h"

// TODO: implement "game is over" logic.
// TODO: enpassant take rule.

// TODO: add all unnecessary getters and setters to comply to OOP nonsense.

#include "move_rules.h"

int main() {
	DisplayBuffer dbuf = DisplayBuffer(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	ChessBoard cb;
	MovementRules mv;
	GameState gm = GameState(&cb, &mv);
	TextDisplay mfu = TextDisplay();

	// FIXME: TMP code
	// #include <sstream>
	std::stringstream debugIn(
		"2E 4E\n"
		"7E 5E\n"
		"1G 3F\n"
		"8B 6C\n"
		"1F 5B\n"
		"8G 6F\n"
		"1E 1G\n"
		"6F 4E\n"
		"1F 1E\n"
		"4E 6D\n"
		"3F 5E\n"
		"8F 7E\n"
		"5B 1F\n"
		"6C 5E\n"
		"1E 5E\n"
		"8E 8G\n"
		"2D 4D\n"
		"7E 6F\n"
		"5E 1E\n"
		"8F 8E\n"
		"2C 3C\n"
		"8E 1E\n"
		"1D 1E\n"
		"6D 8E\n"
		"1C 4F\n"
		"7D 5D\n"
		"1F 3D\n"
		"7G 6G\n"
		"1B 2D\n"
		"8E 7G\n"
		"2D 3F\n"
		"8C 5F\n"
		"3D 5F\n"
		"7G 5F\n"
		"1E 2E\n"
		"7C 6C\n"
		"1A 1E\n"
		"5F 7G\n"
		"4F 5E\n"
		"6F 5E\n"
		"3F 5E\n"
		"8D 6D\n"
		"5E 3D\n"
		"8A 8E\n"
		"2E 2D\n"
		"8E 1E\n"
		"2D 1E\n"
		"7G 6E\n"
		"1E 3E\n"
		"7F 6F\n"
		"2H 4H\n"
		"7H 5H\n"
		"3E 6H\n"
		"8G 7F\n"
		"6H 3E\n"
		"7F 8G\n"
		"3E 6H\n"
		"8G 7F\n"
		"6H 3E\n"
		"7F 8G\n"
		"3E 6H\n"
	);
	// gm.RotatePlayer();
	i32 count = 0;
	// --FIXME: TMP code

	while(!gm.isGameOver) {
		// FIXME: TMP code
		std::cin.get();
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

			std::cin.get();
			while (std::cin.get() != '\n'); // wait for user to press space and ignore any other pressed key.
			continue;
		}

		// Validate Checkmate state:
		if (gm.IsCurrPlayerInCheck() == true) {
			gm.isInCheck = true;

		}

		// Render to screen:
		cb.Display(&dbuf, 2, DISPLAY_CHESS_CENTER_LEFT);
		dbuf.FlushTo(std::cout);

		// Prompt the player for input. This blocks the thread
		// so drawing needs to happen above this line!
		gm.currMovingFrom = {};
		gm.currMovingTo = {};
#if 1
		// FIXME: TMP code
		MoveCmd cmd = MoveCmd(std::cout, debugIn);
		// FIXME: --TMP code

		// FIXME: TMP code
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
}
