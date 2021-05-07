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
	GameState gm = GameState(cb, mv);
	TextDisplay mfu = TextDisplay();

	// FIXME: TMP code
	// #include <sstream>
	std::stringstream debugIn(
		"2A 4A\n"
		"7H 5H\n"
		"4A 5A\n"
		"5H 4H\n"
		"5A 6A\n"
		"4H 3H\n"
		"6A 7A\n"
		"3H 2H\n"
		"7A 8A\n"
		"8E 7E\n"
		"8A 2A\n"
		"2H 1H\n"
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

			std::cin.get();
			while (std::cin.get() != '\n'); // wait for user to press space and ignore any other pressed key.
			continue;
		}

		// Validate Checkmate state:
		gm.CalcOpponentAttackVect();
		if (gm.IsCurrPlayerInCheck() == true) {
			std::cout<<"You are in check"<<std::endl;
			// TODO: add end game check condition here.
		}

		// FIXME: TMP code, This should probably be below the render, but it's here because of the debug ~ render.
		gm.chessBoard.Debug_SetColorsForAttack(gm.opponentAttackVect);
		// --FIXME: TMP code

		// Render to screen:
		cb.Display(dbuf, 2, DISPLAY_CHESS_CENTER_LEFT);
		dbuf.FlushTo(std::cout);

		// Prompt the player for input. This blocks the thread
		// so drawing needs to happen above this line!
		gm.currMovingFrom = {};
		gm.currMovingTo = {};
#if 1
		// FIXME: TMP code
		MoveCmd cmd = MoveCmd(std::cout, debugIn);
		// FIXME: --TMP code
#else
		MoveCmd cmd = MoveCmd(std::cout, std::cin);
#endif

		if (cmd.PromptPlayerInput(gm.currPlayer, gm.currMovingFrom, gm.currMovingTo, gm.errMsg) < 0) {
			continue;
		}

		// Check move validity:
		if (gm.CheckBasicRules() < 0) {
			continue;
		}

		if (gm.IsCurrPlayerInCheck() == true) {
			// TODO: forbit castling here
		}

		// FIXME: TMP code
		// if (cmd.GetUserInput() == "2D 1E") {
		// 	std::cout << "\0";
		// 	// Render to screen:
		// 	cb.Display(dbuf, 2, DISPLAY_CHESS_CENTER_LEFT);
		// 	dbuf.FlushTo(std::cout);
		// }
		// std::cout << cmd.GetUserInput() << " count=" << count++;

		Square& fromSquare = gm.chessBoard.GetSquareAt(gm.currMovingFrom.Row, gm.currMovingFrom.Col);
		Square& toSquare = gm.chessBoard.GetSquareAt(gm.currMovingTo.Row, gm.currMovingTo.Col);
		Piece fromPiece = fromSquare.GetPiece();
		Piece toPiece = toSquare.GetPiece();

		// Check move validity
		std::vector<FieldPos> lMoves;
		mv.PushPieceLegalMoves(cb, fromPiece, gm.currMovingFrom, lMoves);
		bool found = false;
		for (i32 i = 0; i < lMoves.size(); i++) {
			FieldPos cmv = lMoves[i];
			if (cmv == gm.currMovingTo) {
				found = true;
				break;
			}
		}

		// Castle logic:
		bool isCastleMove = gm.IsLegalCastlePos(toSquare.GetPos(), fromPiece.GetPlayerId());
		if (fromPiece.GetType() == PieceType::King && isCastleMove) {
			// Player is making a castle move. We need to move the rook to the special castle place:
			if (toSquare.GetPos() == BOTTOM_LEFT_CASTLE_POS) {
				cb.SwapPieces(FieldPos{7,0}, FieldPos{7, 3});
			}
			else if (toSquare.GetPos() == BOTTOM_RIGHT_CASTLE_POS) {
				cb.SwapPieces(FieldPos{7,7}, FieldPos{7, 5});
			}
			else if (toSquare.GetPos() == TOP_LEFT_CASTLE_POS) {
				cb.SwapPieces(FieldPos{0,0}, FieldPos{0, 3});
			}
			else if (toSquare.GetPos() == TOP_RIGHT_CASTLE_POS) {
				cb.SwapPieces(FieldPos{0,7}, FieldPos{0, 5});
			}
			else {
				assert_exp(!"Castle logic failed");
			}
			found = true;
		}

		if (!found) {
			gm.errMsg = "Piece can NOT move like that";
			continue;
		}

		// Make move:
		cb.SetPieceAt(gm.currMovingTo.Row, gm.currMovingTo.Col, fromPiece);
		cb.SetPieceAt(gm.currMovingFrom.Row, gm.currMovingFrom.Col, EMPTY_PIECE);

		// Recalculate opponent's attack vector after move:
		gm.CalcOpponentAttackVect();

		// Recalculate check validation after a move was made:
		if (gm.IsCurrPlayerInCheck() == true) {
			// Undo move:
			cb.SetPieceAt(gm.currMovingTo.Row, gm.currMovingTo.Col, toPiece);
			cb.SetPieceAt(gm.currMovingFrom.Row, gm.currMovingFrom.Col, fromPiece);
			gm.errMsg = "Move is illegal, because you are in CHECK";
			continue;
		}

		// Check if move is pawn promotion:
		if (fromPiece.GetType() == PieceType::Pawn && gm.currPlayer == 1 && gm.currMovingTo.Row == 0) {
			cb.SetPieceAt(gm.currMovingTo.Row, gm.currMovingTo.Col, Piece(PieceType::Queen, gm.currPlayer));
		}
		if (fromPiece.GetType() == PieceType::Pawn && gm.currPlayer == 2 && gm.currMovingTo.Row == 7) {
			cb.SetPieceAt(gm.currMovingTo.Row, gm.currMovingTo.Col, Piece(PieceType::Queen, gm.currPlayer));
		}

		// After all validation, mark squares as no longer holding theier original pieces.
		fromSquare.OriginalPieceMoved();
		toSquare.OriginalPieceMoved();

		// --FIXME: TMP code

		// Exact correct place for player rotation (do NOT move):
		gm.RotatePlayer();
	}
}
