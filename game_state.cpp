#include "game_state.h"

FieldPos findKingPos(const std::vector<Square*> _squares);

GameState::GameState(ChessBoard *_cb, MovementRules *_mv) {
	assert_exp(chessBoard != nullptr);
	assert_exp(movementRules != nullptr);
	chessBoard = _cb;
	movementRules= _mv;
}
GameState::~GameState() {}
u32 GameState::GetCurrPlayer() { return currPlayer; }
u32 GameState::GetOpponentPlayer() {
	u32 ret = ((currPlayer == 1) ? 2 : 1);
	return ret;
}
void GameState::RotatePlayer() {
	currPlayer = this->GetOpponentPlayer();
}
void GameState::Init() {
	currMovingFrom = {};
	currMovingTo = {};
	isInCheck = false;
}

bool GameState::CheckBasicRules() {
	Piece fromPiece = this->chessBoard->GetPieceAt(currMovingFrom);
	if (fromPiece.IsEmpty()) {
		errMsg = "There is not piece at that location.";
		return false;
	}
	if (currPlayer != fromPiece.GetPlayerId()) {
		errMsg = "This piece does NOT belong to you.";
		return false;
	}

	Piece toPiece = this->chessBoard->GetPieceAt(currMovingTo);
	if (currPlayer == toPiece.GetPlayerId()) {
		errMsg = "You can NOT take your own piece.";
		return false;
	}

	return true;
}

bool GameState::IsOpponentAttackingAt(i32 _row, i32 _col) {
	std::vector<Square*> ocs; // opponent controlled squares.
	chessBoard->GetPlayerSquares(this->GetOpponentPlayer(), ocs);
	std::vector<FieldPos> av; // attack vector

	for (i32 i = 0; i < ocs.size(); i++) {
		Square* s = ocs[i];
		FieldPos pos = s->GetPos();
		Piece piece = s->GetPiece();

		movementRules->PushPieceLegalAttacks(*chessBoard, piece, pos, av);
		for (i32 j = 0; j < av.size(); j++) {
			if (av[j].Row == _row && av[j].Col == _col) {
				return true;
			}
		}

		av.clear();
	}

	return false;
}

bool GameState::IsOpponentAttackingAt(FieldPos _p) {
	return this->IsOpponentAttackingAt(_p.Row, _p.Col);
}

bool GameState::IsCurrPlayerInCheck() {
	std::vector<Square*> ccs;
	chessBoard->GetPlayerSquares(this->GetCurrPlayer(), ccs);
	FieldPos kingPos = findKingPos(ccs);
	bool ret = this->IsOpponentAttackingAt(kingPos);
	return ret;
}

bool GameState::IsLegalCastlePos(FieldPos _pos, u32 _playerId) {
	if (isInCheck) {
		return false; // no castle when in check.
	}

	bool ret = false;
	if (_playerId == 1) {
		bool kingIsAtOriginSpot = (chessBoard->GetSquareAt(7, 4)->HasOriginalPiece() &&
									chessBoard->GetPieceTypeAt(7, 4) == PieceType::King);
		if (kingIsAtOriginSpot) {
			bool pathToKingLeft = (chessBoard->GetPieceTypeAt(7, 1) == PieceType::None &&
									chessBoard->GetPieceTypeAt(7, 2) == PieceType::None &&
									chessBoard->GetPieceTypeAt(7, 3) == PieceType::None);
			bool leftIsNOTAttaceked = (!this->IsOpponentAttackingAt(7, 1) &&
										!this->IsOpponentAttackingAt(7, 2) &&
										!this->IsOpponentAttackingAt(7, 3));
			bool leftRookIsAtOriginSpot = (chessBoard->GetSquareAt(7, 0)->HasOriginalPiece() &&
											chessBoard->GetPieceTypeAt(7, 0) == PieceType::Rook);
			bool leftCastle = (_pos == BOTTOM_LEFT_CASTLE_POS && leftRookIsAtOriginSpot && leftIsNOTAttaceked);

			bool pathToKingRight = (chessBoard->GetPieceTypeAt(7, 6) == PieceType::None &&
									chessBoard->GetPieceTypeAt(7, 5) == PieceType::None);
			bool rightIsNOTAttaceked = (!this->IsOpponentAttackingAt(7, 6) &&
										!this->IsOpponentAttackingAt(7, 5));
			bool rightRookIsAtOriginSpot = (chessBoard->GetSquareAt(7, 7)->HasOriginalPiece() &&
											chessBoard->GetPieceTypeAt(7, 7) == PieceType::Rook);
			bool rightCastle = (_pos == BOTTOM_RIGHT_CASTLE_POS && rightRookIsAtOriginSpot && rightIsNOTAttaceked);

			ret = ((leftCastle && pathToKingLeft) || (rightCastle && pathToKingRight));
		}
	} else if (_playerId == 2) {
		bool kingIsAtOriginSpot = (chessBoard->GetSquareAt(0, 4)->HasOriginalPiece() &&
									chessBoard->GetPieceTypeAt(0, 4) == PieceType::King);
		if (kingIsAtOriginSpot) {
			bool pathToKingLeft = (chessBoard->GetPieceTypeAt(0, 1) == PieceType::None &&
									chessBoard->GetPieceTypeAt(0, 2) == PieceType::None &&
									chessBoard->GetPieceTypeAt(0, 3) == PieceType::None);
			bool leftIsNOTAttaceked = (!this->IsOpponentAttackingAt(0, 1) &&
										!this->IsOpponentAttackingAt(0, 2) &&
										!this->IsOpponentAttackingAt(0, 3));
			bool leftRookIsAtOriginSpot = (chessBoard->GetSquareAt(0, 0)->HasOriginalPiece() &&
											chessBoard->GetPieceTypeAt(0, 0) == PieceType::Rook);
			bool leftCastle = (_pos == TOP_LEFT_CASTLE_POS && leftRookIsAtOriginSpot && leftIsNOTAttaceked);

			bool pathToKingRight = (chessBoard->GetPieceTypeAt(0, 6) == PieceType::None &&
									chessBoard->GetPieceTypeAt(0, 5) == PieceType::None);
			bool rightIsNOTAttaceked = (!this->IsOpponentAttackingAt(0, 6) &&
										!this->IsOpponentAttackingAt(0, 5));
			bool rightRookIsAtOriginSpot = (chessBoard->GetSquareAt(0, 7)->HasOriginalPiece() &&
											chessBoard->GetPieceTypeAt(0, 7) == PieceType::Rook);
			bool rightCastle = (_pos == TOP_RIGHT_CASTLE_POS && rightRookIsAtOriginSpot && rightIsNOTAttaceked);

			ret = ((leftCastle && pathToKingLeft) || (rightCastle && pathToKingRight));
		}
	}
	return ret;
}

bool GameState::IsCurrMoveLegal() {
	bool validMove = false;

	// Check normal move validity
	std::vector<FieldPos> lMoves;
	Piece fromPiece = this->chessBoard->GetPieceAt(currMovingFrom.Row, currMovingFrom.Col);
	this->movementRules->PushPieceLegalMoves(*chessBoard, fromPiece, this->currMovingFrom, lMoves);
	for (i32 i = 0; i < lMoves.size(); i++) {
		FieldPos cmv = lMoves[i];
		if (cmv == this->currMovingTo) {
			validMove = true;
			break;
		}
	}

	// Castle move logic:
	if (fromPiece.GetType() == PieceType::King && validMove == false) {
		// Player is making a castle move. We need to move the rook to the special castle place:
		// IsLegalCastlePos is a bit expensive, so calculate it only in the 4 special cases.
		bool isValidCastleMove;
		if (currMovingTo == BOTTOM_LEFT_CASTLE_POS) {
			isValidCastleMove = this->IsLegalCastlePos(currMovingTo, fromPiece.GetPlayerId());
			chessBoard->SwapPieces(FieldPos{7,0}, FieldPos{7, 3});
		}
		else if (currMovingTo == BOTTOM_RIGHT_CASTLE_POS) {
			isValidCastleMove = this->IsLegalCastlePos(currMovingTo, fromPiece.GetPlayerId());
			chessBoard->SwapPieces(FieldPos{7,7}, FieldPos{7, 5});
		}
		else if (currMovingTo == TOP_LEFT_CASTLE_POS) {
			isValidCastleMove = this->IsLegalCastlePos(currMovingTo, fromPiece.GetPlayerId());
			chessBoard->SwapPieces(FieldPos{0,0}, FieldPos{0, 3});
		}
		else if (currMovingTo == TOP_RIGHT_CASTLE_POS) {
			isValidCastleMove = this->IsLegalCastlePos(currMovingTo, fromPiece.GetPlayerId());
			chessBoard->SwapPieces(FieldPos{0,7}, FieldPos{0, 5});
		}
		validMove = isValidCastleMove;
	}

	if (!validMove) {
		this->errMsg = "Piece can NOT move like that";
	}

	return validMove;
}

bool GameState::TryMakeMove() {
	Square *fromSquare = chessBoard->GetSquareAt(currMovingFrom);
	Square *toSquare = chessBoard->GetSquareAt(currMovingTo);
	Piece fromPiece = fromSquare->GetPiece();
	Piece toPiece = toSquare->GetPiece();
	chessBoard->SetPieceAt(currMovingTo, &fromPiece);
	chessBoard->SetPieceAt(currMovingFrom, &EMPTY_PIECE);

	// If player is in check after move, we need to undo the move:
	if (this->IsCurrPlayerInCheck() == true) {
		this->isInCheck = true;
		chessBoard->SetPieceAt(this->currMovingTo, &toPiece);
		chessBoard->SetPieceAt(this->currMovingFrom, &fromPiece);
		this->errMsg = "Move is illegal, because you are in CHECK";
		return false;
	}

	// Check if move is pawn promotion:
	if (fromPiece.GetType() == PieceType::Pawn && this->GetCurrPlayer() == 1 && this->currMovingTo.Row == 0) {
		Piece promotedPiece = Piece(PieceType::Queen, this->GetCurrPlayer());
		chessBoard->SetPieceAt(this->currMovingTo, &promotedPiece);
	}
	if (fromPiece.GetType() == PieceType::Pawn && this->GetCurrPlayer() == 2 && this->currMovingTo.Row == 7) {
		Piece promotedPiece = Piece(PieceType::Queen, this->GetCurrPlayer());
		chessBoard->SetPieceAt(this->currMovingTo, &promotedPiece);
	}

	// After the move mark squares as no longer holding theier original pieces.
	fromSquare->OriginalPieceMoved();
	toSquare->OriginalPieceMoved();

	return true;
}

FieldPos findKingPos(const std::vector<Square*> _squares) {
	for (i32 i = 0; i < _squares.size(); i++) {
		Square *sqr = _squares[i];
		assert_exp(sqr != nullptr); // sanity check.
		Piece p = sqr->GetPiece();
		if (p.GetType() == PieceType::King) {
			return sqr->GetPos();
		}
	}

	assert_exp(!"Should always have a king");
	return ZERO_POS;
}