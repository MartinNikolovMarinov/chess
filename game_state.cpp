#include "game_state.h"

GameState::GameState(ChessBoard &_cb, MovementRules &_mv) : chessBoard(_cb), movementRules(_mv) {}
GameState::~GameState() {}
void GameState::RotatePlayer() { currPlayer = ((currPlayer == 1) ? 2 : 1); }
void GameState::Clear() {
	cmdInputLine.clear();
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

void GameState::GetPlayerSquares(u32 _pid, std::vector<Square*> &_out) {
	for (i32 row = 0; row < FIELD_SIZE; row++) {
		for (i32 col = 0; col < FIELD_SIZE; col++) {
			Square& sqr = chessBoard.GetSquareAt(row, col);
			Piece& p = sqr.GetPiece();
			if (p.GetPlayerId() == _pid) {
				_out.push_back(&sqr);
			}
		}
	}
}

void GameState::CalcOpponentAttackVect() {
	std::vector<Square*> squares;
	u32 opponentId = currPlayer == 1 ? 2 : 1;
	this->GetPlayerSquares(opponentId, squares);

	for (i32 i = 0; i < squares.size(); i++) {
		Square *sqr = squares[i];
		assert_exp(sqr != nullptr);
		Piece& p = sqr->GetPiece();
		FieldPos pos = sqr->GetPos();
		movementRules.PushPieceLegalAttacks(chessBoard, p, pos,  opponentAttackVect);
	}
}

bool GameState::IsCurrPlayerInCheck() {
	auto oattv = this->opponentAttackVect;
	std::vector<Square*> squares;
	this->GetPlayerSquares(currPlayer, squares);
	FieldPos kingPos = FindKingSquare(squares);

	for (i32 i = 0; i < oattv.size(); i++) {
		FieldPos attackedPos = oattv[i];
		if (attackedPos == kingPos) {
			return true;
		}
	}

	return false;
}

FieldPos FindKingSquare(const std::vector<Square*> _squares) {
	for (i32 i = 0; i < _squares.size(); i++) {
		Square *sqr = _squares[i];
		assert_exp(sqr != nullptr);
		Piece &p = sqr->GetPiece();
		FieldPos &fp = sqr->GetPos();
		if (p.GetType() == PieceType::King) {
			return fp;
		}
	}

	assert_exp(!"Should always have a king");
	return (FieldPos&)ZERO_POS;
}