#include "square.h"

Square::Square() {}
Square::Square(u32 _w, u32 _h, SquareColor _c, const Piece &_p, const FieldPos &_pos)
	: color(_c)
{
	// We need at least 3 by 3 square to be able to display a pieace in the square:
	assert_exp(_w >= 3);
	assert_exp(_h >= 3);

	width = _w;
	height = _h;
	pos.Row = _pos.Row;
	pos.Col = _pos.Col;
	this->SetPiece(_p);
	hasOriginalPiece = true;
}
Square::~Square() {}

bool Square::GetEnPassant() { return enPassant; }
void Square::SetEnPassant(bool _v) { enPassant = _v; }
SquareColor Square::GetColor() { return color; }
void Square::SetColor(SquareColor _c) { color = _c; }
void Square::SetPiece(const Piece &_p) { piece = Piece(_p); }
Piece& Square::GetPiece() { return piece; }
FieldPos& Square::GetPos() { return pos; }
void Square::OriginalPieceMoved() { hasOriginalPiece = false; }
bool Square::HasOriginalPiece() { return hasOriginalPiece; }

void Square::Display(DisplayBuffer &_dbuf, u32 _top, u32 _left) {
	Displayer::Display(_dbuf, _top, _left);

	assert_exp(height + _top <= _dbuf.GetHeight());
	assert_exp(width + _left <= _dbuf.GetWidth());

	// Draw top
	for (i32 col = 0; col < width; col++) {
		u32 offRow = 0 + _top;
		u32 offCol = col + _left;
		if (col == 0 || col == width - 1) _dbuf.SetAt(offRow, offCol, '|');
		else _dbuf.SetAt(offRow, offCol, '-');
	}

	// Draw center
	for (i32 row = 1; row < height - 1; row++) {
		for (i32 col = 0; col < width; col++) {
			u32 offRow = row + _top;
			u32 offCol = col + _left;
			if (col == 0 || col == width - 1) _dbuf.SetAt(offRow, offCol, '|');
			else _dbuf.SetAt(offRow, offCol, ' ');
		}
	}

	if (this->piece.GetPlayerId() > 0 && this->piece.GetType() != PieceType::None) {
		// Display piece right in the center of the square:
		u32 pRow = (height / 2) + _top;
		u32 pCol = (width / 2) + _left - 1;
		_dbuf.SetAt(pRow, pCol, U32DigitToChar(this->piece.GetPlayerId()));
		_dbuf.SetAt(pRow, pCol + 1, this->piece.GetType());
	}

	// Draw bottom
	for (i32 col = 0; col < width; col++) {
		u32 offRow = (height - 1) + _top;
		u32 offCol = col + _left;
		if (col == 0 || col == width - 1) _dbuf.SetAt(offRow, offCol, '|');
		else _dbuf.SetAt(offRow, offCol, '-');
	}

	// Draw color indication
	switch (this->color) {
		// FIXME: DEBUG:
		case SquareColor::White:
		case SquareColor::Black:
			// _dbuf.SetAt(_top + 1, _left + 2, U32DigitToChar(this->piece.GetPlayerId()));
			break;
		case SquareColor::Debug:
			// _dbuf.SetAt(_top + 1, _left + 2, U32DigitToChar(this->piece.GetPlayerId()));
			_dbuf.SetAt(_top + 1, _left + 1, '~');
			break;
		// --FIXME: DEBUG:

		// case SquareColor::Black:
		// 	_dbuf.SetAt(_top + 1, _left + 1, '*');
		// 	break;
		default:
			break;
	}
}