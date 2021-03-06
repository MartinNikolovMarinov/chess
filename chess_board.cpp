#include "chess_board.h"

ChessBoard::ChessBoard() {
	Piece emptyPiece;
	bool isWhite = true;
	for (i32 row = 0; row < FIELD_SIZE; row++) {
		for (i32 col = 0; col < FIELD_SIZE; col++) {
			SquareColor color = isWhite ? SquareColor::White : SquareColor::Black;
			FieldPos pos = {row, col};
			field[row][col] = Square(SQUARE_WIDTH, SQUARE_HEIGHT, color, &emptyPiece, pos);
			isWhite = !isWhite;
		}

		isWhite = !isWhite;
	}

	this->initBoardState();
}
ChessBoard::~ChessBoard() {}

Piece ChessBoard::GetPieceAt(i32 _row, i32 _col) {
	assert_exp(this->IsInRange(_row, _col));
	Piece ret = field[_row][_col].GetPiece();
	return ret;
}
Piece ChessBoard::GetPieceAt(FieldPos _pos) {
	return this->GetPieceAt(_pos.Row, _pos.Col);
}
void ChessBoard::SetPieceAt(i32 _row, i32 _col, const Piece *_p) {
	assert_exp(this->IsInRange(_row, _col));
	field[_row][_col].SetPiece(_p);
}
void ChessBoard::SetPieceAt(FieldPos _pos, const Piece *_p) {
	return this->SetPieceAt(_pos.Row, _pos.Col, _p);
}
PieceType ChessBoard::GetPieceTypeAt(i32 _row, i32 _col) {
	Piece p = this->GetPieceAt(_row, _col);
	PieceType ret = p.GetType();
	return ret;
}
PieceType ChessBoard::GetPieceTypeAt(FieldPos _pos) {
	return this->GetPieceTypeAt(_pos.Row, _pos.Col);
}
Square* ChessBoard::GetSquareAt(i32 _row, i32 _col) {
	assert_exp(this->IsInRange(_row, _col));
	Square *sqr= &field[_row][_col];
	return sqr;
}
Square* ChessBoard::GetSquareAt(FieldPos _pos) {
	return this->GetSquareAt(_pos.Row, _pos.Col);
}

void ChessBoard::GetPlayerSquares(u32 _pid, std::vector<Square*> &_out) {
	for (i32 row = 0; row < FIELD_SIZE; row++) {
		for (i32 col = 0; col < FIELD_SIZE; col++) {
			Square *sqr= &field[row][col];
			Piece p = sqr->GetPiece();
			if (p.GetPlayerId() == _pid) {
				_out.push_back(sqr);
			}
		}
	}
}

bool ChessBoard::IsInRange(i32 _row, i32 _col) {
	bool rowInRange = 0 <= _row && _row < FIELD_SIZE;
	bool colInRange = 0 <= _col && _col < FIELD_SIZE;
	return rowInRange && colInRange;
}
bool ChessBoard::IsInRange(FieldPos _pos) {
	return this->IsInRange(_pos.Row, _pos.Col);
}
bool ChessBoard::IsOwnedByOpponent(u32 playerId, i32 _row, i32 _col) {
	if (!this->IsInRange(_row, _col)) {
		// invalid coords are not an error for this function!
		return false;
	}
	Piece p = this->GetPieceAt(_row, _col);
	bool ret = p.GetPlayerId() != playerId && p.GetPlayerId() != 0;
	return ret;
}
bool ChessBoard::IsOwnedByOpponent(u32 playerId, FieldPos _pos) {
	return this->IsOwnedByOpponent(playerId, _pos.Row, _pos.Col);
}

bool ChessBoard::canAttackSquare(u32 _playerId, i32 _row, i32 _col) {
	if (this->IsInRange(_row, _col) == false) {
		return false;
	}
	Piece currPiece = this->GetPieceAt(_row, _col);
	bool currPieceIsNone = (currPiece.GetType() == PieceType::None);
	bool notOurPiece = currPiece.GetPlayerId() != _playerId;
	return (currPieceIsNone || notOurPiece);
}

void ChessBoard::SwapPieces(FieldPos _first, FieldPos _second) {
	Piece f = this->GetPieceAt(_first);
	Piece s = this->GetPieceAt(_second);
	this->SetPieceAt(_second, &f);
	this->SetPieceAt(_first, &s);
}

void ChessBoard::PushIfAttackPossible(u32 _pid, i32 _row, i32 _col, std::vector<FieldPos> &_av) {
	bool canAttack = this->canAttackSquare(_pid, _row, _col);
	if (canAttack) {
		FieldPos pos = {_row, _col};
		_av.push_back(pos);
	}
}

void ChessBoard::CalcAttackVector(FieldPos _from, FieldPos _to, FieldPos _direction, std::vector<FieldPos> &_av) {
	Piece subjectPiece = this->GetPieceAt(_from.Row, _from.Col);
	assert_exp(subjectPiece.GetType() != PieceType::None);

	u32 playerId = subjectPiece.GetPlayerId();
	FieldPos currPos = (_from + _direction) ;
	bool reachedDest = false;
	bool reachedOccupiedPos = false;
	while(!reachedDest && !reachedOccupiedPos) {
		this->PushIfAttackPossible(playerId, currPos.Row, currPos.Col, _av);
		if (this->IsInRange(currPos) == false) {
			break;
		}
		reachedOccupiedPos = (this->GetPieceTypeAt(currPos.Row, currPos.Col) != PieceType::None);
		reachedDest = (currPos == _to);
		currPos += _direction;
	}
}

void ChessBoard::Display(DisplayBuffer *_dbuf, u32 _top, u32 _left) {
	Displayer::Display(_dbuf, _top, _left);

	// Draw numbers on top of chess board:
	for (i32 i = 0; i < FIELD_SIZE; i++) {
		u32 offRow = _top - 1;
		u32 offCol = (i * SQUARE_WIDTH) + _left + (SQUARE_WIDTH / 2);
		_dbuf->SetAt(offRow, offCol, CanonicalPosToChessLetter(i));
	}

	// Draw chess board:
	/* NOTE:
		If we just draw the squares right next to each other the boarders of the
		squares are drawn twice. The offsetForNPlusOne compensates for this, by
		drawing every (n+1) square one possition above where it should be.
		This removes padding between the above square.
	*/
	u32 offsetForNPlusOne = 0;
	for (i32 row = 0; row < FIELD_SIZE; row++) {
		for (i32 col = 0; col < FIELD_SIZE; col++) {
			Square &curr = field[row][col];
			u32 offRow = (row * SQUARE_HEIGHT) + _top - offsetForNPlusOne;
			u32 offCol = (col * SQUARE_WIDTH) + _left;
			curr.Display(_dbuf, offRow, offCol);
		}

		offsetForNPlusOne++;
	}

	// Draw letters next to chess board:
	offsetForNPlusOne = 0;
	for (i32 i = 0; i < FIELD_SIZE; i++) {
		u32 offRow = (i * SQUARE_HEIGHT) + (_top - offsetForNPlusOne) + (SQUARE_HEIGHT / 2);
		u32 offCol = _left - 2;
		_dbuf->SetAt(offRow, offCol, U32DigitToChar(FIELD_SIZE - i));
		offsetForNPlusOne++;
	}
}

void ChessBoard::initBoardState() {
	Piece p;

	p = Piece(PieceType::Rook, 1);
	field[FIELD_SIZE - 1][0].SetPiece(&p);
	p = Piece(PieceType::Knight, 1);
	field[FIELD_SIZE - 1][1].SetPiece(&p);
	p = Piece(PieceType::Bishop, 1);
	field[FIELD_SIZE - 1][2].SetPiece(&p);
	p = Piece(PieceType::Queen, 1);
	field[FIELD_SIZE - 1][3].SetPiece(&p);
	p = Piece(PieceType::King, 1);
	field[FIELD_SIZE - 1][4].SetPiece(&p);
	p = Piece(PieceType::Bishop, 1);
	field[FIELD_SIZE - 1][5].SetPiece(&p);
	p = Piece(PieceType::Knight, 1);
	field[FIELD_SIZE - 1][6].SetPiece(&p);
	p = Piece(PieceType::Rook, 1);
	field[FIELD_SIZE - 1][7].SetPiece(&p);

	p = Piece(PieceType::Pawn, 1);
	field[FIELD_SIZE - 2][0].SetPiece(&p);
	p = Piece(PieceType::Pawn, 1);
	field[FIELD_SIZE - 2][1].SetPiece(&p);
	p = Piece(PieceType::Pawn, 1);
	field[FIELD_SIZE - 2][2].SetPiece(&p);
	p = Piece(PieceType::Pawn, 1);
	field[FIELD_SIZE - 2][3].SetPiece(&p);
	p = Piece(PieceType::Pawn, 1);
	field[FIELD_SIZE - 2][4].SetPiece(&p);
	p = Piece(PieceType::Pawn, 1);
	field[FIELD_SIZE - 2][5].SetPiece(&p);
	p = Piece(PieceType::Pawn, 1);
	field[FIELD_SIZE - 2][6].SetPiece(&p);
	p = Piece(PieceType::Pawn, 1);
	field[FIELD_SIZE - 2][7].SetPiece(&p);

	p = Piece(PieceType::Rook, 2);
	field[0][0].SetPiece(&p);
	p = Piece(PieceType::Knight, 2);
	field[0][1].SetPiece(&p);
	p = Piece(PieceType::Bishop, 2);
	field[0][2].SetPiece(&p);
	p = Piece(PieceType::Queen, 2);
	field[0][3].SetPiece(&p);
	p = Piece(PieceType::King, 2);
	field[0][4].SetPiece(&p);
	p = Piece(PieceType::Bishop, 2);
	field[0][5].SetPiece(&p);
	p = Piece(PieceType::Knight, 2);
	field[0][6].SetPiece(&p);
	p = Piece(PieceType::Rook, 2);
	field[0][7].SetPiece(&p);

	p = Piece(PieceType::Pawn, 2);
	field[1][0].SetPiece(&p);
	p = Piece(PieceType::Pawn, 2);
	field[1][1].SetPiece(&p);
	p = Piece(PieceType::Pawn, 2);
	field[1][2].SetPiece(&p);
	p = Piece(PieceType::Pawn, 2);
	field[1][3].SetPiece(&p);
	p = Piece(PieceType::Pawn, 2);
	field[1][4].SetPiece(&p);
	p = Piece(PieceType::Pawn, 2);
	field[1][5].SetPiece(&p);
	p = Piece(PieceType::Pawn, 2);
	field[1][6].SetPiece(&p);
	p = Piece(PieceType::Pawn, 2);
	field[1][7].SetPiece(&p);
}