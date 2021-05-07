#include "game_state.h"

GameState::GameState(ChessBoard &_cb, MovementRules &_mv) : chessBoard(_cb), movementRules(_mv) {}
GameState::~GameState() {}
void GameState::RotatePlayer() { currPlayer = ((currPlayer == 1) ? 2 : 1); }
void GameState::Init() {
	opponentAttackVect.clear();
}

i32 GameState::CheckBasicRules() {
	Piece fromPiece = this->chessBoard.GetPieceAt(currMovingFrom);
	if (fromPiece.IsEmpty()) {
		errMsg = "There is not piece at that location.";
		return -1;
	}
	if (currPlayer != fromPiece.GetPlayerId()) {
		errMsg = "This piece does NOT belong to you.";
		return -1;
	}

	Piece toPiece = this->chessBoard.GetPieceAt(currMovingTo);
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
	opponentAttackVect.clear();
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

bool GameState::IsSquareUnderAttack(i32 _row, i32 _col) {
	bool ret = false;
	for (i32 i = 0; i < opponentAttackVect.size(); i++) {
		FieldPos curr = opponentAttackVect[i];
		if (curr.Row == _row && curr.Col == _col) {
			ret = true;
			break;
		}
	}
	return ret;
}

bool GameState::IsSquareUnderAttack(const FieldPos &p) {
	return this->IsSquareUnderAttack(p.Row, p.Col);
}

bool GameState::IsLegalCastlePos(const FieldPos &_pos, u32 _playerId) {
	bool ret = false;
	if (_playerId == 1) {
		bool kingIsAtOriginSpot = (this->chessBoard.GetSquareAt(7, 4).HasOriginalPiece() &&
									this->chessBoard.GetPieceTypeAt(7, 4) == PieceType::King);
		if (kingIsAtOriginSpot) {
			bool pathToKingLeft = (this->chessBoard.GetPieceTypeAt(7, 1) == PieceType::None &&
									this->chessBoard.GetPieceTypeAt(7, 2) == PieceType::None &&
									this->chessBoard.GetPieceTypeAt(7, 3) == PieceType::None);
			bool leftIsNOTAttaceked = (!this->IsSquareUnderAttack(7, 1) &&
										!this->IsSquareUnderAttack(7, 2) &&
										!this->IsSquareUnderAttack(7, 3));
			bool leftRookIsAtOriginSpot = (this->chessBoard.GetSquareAt(7, 0).HasOriginalPiece() &&
											this->chessBoard.GetPieceTypeAt(7, 0) == PieceType::Rook);
			bool leftCastle = (_pos == BOTTOM_LEFT_CASTLE_POS && leftRookIsAtOriginSpot && leftIsNOTAttaceked);

			bool pathToKingRight = (this->chessBoard.GetPieceTypeAt(7, 6) == PieceType::None &&
									this->chessBoard.GetPieceTypeAt(7, 5) == PieceType::None);
			bool rightIsNOTAttaceked = (!this->IsSquareUnderAttack(7, 6) &&
										!this->IsSquareUnderAttack(7, 5));
			bool rightRookIsAtOriginSpot = (this->chessBoard.GetSquareAt(7, 7).HasOriginalPiece() &&
											this->chessBoard.GetPieceTypeAt(7, 7) == PieceType::Rook);
			bool rightCastle = (_pos == BOTTOM_RIGHT_CASTLE_POS && rightRookIsAtOriginSpot && rightIsNOTAttaceked);

			ret = ((leftCastle && pathToKingLeft) || (rightCastle && pathToKingRight));
		}
	} else if (_playerId == 2) {
		bool kingIsAtOriginSpot = (this->chessBoard.GetSquareAt(0, 4).HasOriginalPiece() &&
									this->chessBoard.GetPieceTypeAt(0, 4) == PieceType::King);
		if (kingIsAtOriginSpot) {
			bool pathToKingLeft = (this->chessBoard.GetPieceTypeAt(0, 1) == PieceType::None &&
									this->chessBoard.GetPieceTypeAt(0, 2) == PieceType::None &&
									this->chessBoard.GetPieceTypeAt(0, 3) == PieceType::None);
			bool leftIsNOTAttaceked = (!this->IsSquareUnderAttack(0, 1) &&
										!this->IsSquareUnderAttack(0, 2) &&
										!this->IsSquareUnderAttack(0, 3));
			bool leftRookIsAtOriginSpot = (this->chessBoard.GetSquareAt(0, 0).HasOriginalPiece() &&
											this->chessBoard.GetPieceTypeAt(0, 0) == PieceType::Rook);
			bool leftCastle = (_pos == TOP_LEFT_CASTLE_POS && leftRookIsAtOriginSpot && leftIsNOTAttaceked);

			bool pathToKingRight = (this->chessBoard.GetPieceTypeAt(0, 6) == PieceType::None &&
									this->chessBoard.GetPieceTypeAt(0, 5) == PieceType::None);
			bool rightIsNOTAttaceked = (!this->IsSquareUnderAttack(0, 6) &&
										!this->IsSquareUnderAttack(0, 5));
			bool rightRookIsAtOriginSpot = (this->chessBoard.GetSquareAt(0, 7).HasOriginalPiece() &&
											this->chessBoard.GetPieceTypeAt(0, 7) == PieceType::Rook);
			bool rightCastle = (_pos == TOP_RIGHT_CASTLE_POS && rightRookIsAtOriginSpot && rightIsNOTAttaceked);

			ret = ((leftCastle && pathToKingLeft) || (rightCastle && pathToKingRight));
		}
	}
	return ret;
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