#include "game_state.h"

GameState::GameState(ChessBoard &_cb, MovementRules &_mv) : chessBoard(_cb), movementRules(_mv) {}
GameState::~GameState() {}
void GameState::RotatePlayer() { currPlayer = ((currPlayer == 1) ? 2 : 1); }
void GameState::Clear() {
	inputLine.clear();
	currPlayerAttackVect.clear();
}

Piece& GameState::GetFromPiece() {
	Piece& ret = chessBoard.GetPieceAt(currMovingFrom.Row, currMovingFrom.Col);
	return ret;
}

Piece& GameState::GetToPiece() {
	Piece& ret = chessBoard.GetPieceAt(currMovingTo.Row, currMovingTo.Col);
	return ret;
}

i32 GameState::CheckBasicRules() {
	Piece fromPiece = GetFromPiece();
	if (fromPiece.IsEmpty()) {
		errMsg = "There is not piece at that location.";
		return -1;
	}
	if (currPlayer != fromPiece.GetPlayerId()) {
		errMsg = "This piece does NOT belong to you.";
		return -1;
	}

	Piece toPiece = GetToPiece();
	if (currPlayer == toPiece.GetPlayerId()) {
		errMsg = "You can NOT take your own piece.";
		return -1;
	}

	return 0;
}

void GameState::CalcCurrUserAttackVect() {
	for (i32 row = 0; row < FIELD_SIZE; row++) {
		for (i32 col = 0; col < FIELD_SIZE; col++) {
			Piece &p = chessBoard.GetPieceAt(row, col);
			if (p.GetPlayerId() == currPlayer) {
				// currPlayerPieces.push_back(p);
				MovePos pos = {row, col};
				movementRules.PushPieceLegalAttacks(chessBoard, p, pos, currPlayerAttackVect);
			}
		}
	}

	// FIXME: TMP code
	chessBoard.Debug_SetColorsForAttack(currPlayerAttackVect);
	// --FIXME: TMP code
}