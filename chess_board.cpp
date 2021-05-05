#include "chess_board.h"

ChessBoard::ChessBoard() {
	Piece emptyPiece;
	bool isWhite = true;
	for (i32 row = 0; row < FIELD_SIZE; row++) {
		for (i32 col = 0; col < FIELD_SIZE; col++) {
			SquareColor color = isWhite ? SquareColor::White : SquareColor::Black;
			field[row][col] = Square(SQUARE_WIDTH, SQUARE_HEIGHT, color, emptyPiece);
			isWhite = !isWhite;
		}

		isWhite = !isWhite;
	}

	initBoardState();
}
ChessBoard::~ChessBoard() {}

const Piece& ChessBoard::GetPieceAt(i32 _row, i32 _col) const {
	assert_exp(this->IsInRange(_row, _col));
	const Piece &ret = field[_row][_col].GetPiece();
	return ret;
}

PieceType ChessBoard::GetPieceTypeAt(i32 _row, i32 _col) const {
	const Piece &p = this->GetPieceAt(_row, _col);
	PieceType ret = p.GetType();
	return ret;
}

bool ChessBoard::IsInRange(i32 _row, i32 _col) const {
	bool rowInRange = 0 <= _row && _row < FIELD_SIZE;
	bool colInRange = 0 <= _col && _col < FIELD_SIZE;
	return rowInRange && colInRange;
}

void ChessBoard::Display(DisplayBuffer &_dbuf, u32 _top, u32 _left) {
	Displayer::Display(_dbuf, _top, _left);

	// Draw numbers on top of chess board:
	for (i32 i = 0; i < FIELD_SIZE; i++) {
		u32 offRow = _top - 1;
		u32 offCol = i * SQUARE_WIDTH + _left + (SQUARE_WIDTH / 2);
		_dbuf.SetAt(offRow, offCol, CanonicalPosToChessLetter(i));
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
			Square curr = field[row][col];
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
		_dbuf.SetAt(offRow, offCol, U32DigitToChar(FIELD_SIZE - i));
		offsetForNPlusOne++;
	}
}

void ChessBoard::initBoardState() {
#if 1
	/* FIXME:
		This slow/bad code is here for debugging convenience, until the final algorithm is done!
	*/

	// std::string rawField =
	// 	"2R 2N 2B 2Q 2K 2B 2N 2R\n"
	// 	"2P 2P 2P 2P 2P 2P 2P 2P\n"
	// 	"00 00 00 00 00 00 00 00\n"
	// 	"00 00 00 00 00 00 00 00\n"
	// 	"00 00 00 00 00 00 00 00\n"
	// 	"00 00 00 00 00 00 00 00\n"
	// 	"1P 1P 1P 1P 1P 1P 1P 1P\n"
	// 	"1R 1N 1B 1Q 1K 1B 1N 1R";

	std::string rawField =
		"00 00 00 00 00 00 00 00\n"
		"00 00 00 00 00 00 00 00\n"
		"00 00 00 00 00 00 00 00\n"
		"00 00 00 00 00 00 00 00\n"
		"00 00 00 00 00 00 00 00\n"
		"00 00 00 00 00 00 1P 00\n"
		"00 00 00 00 00 2P 00 00\n"
		"00 00 00 00 00 00 00 1N";

	auto splitVect = Debug_StrSplit(rawField, "\n");
	for (i32 row = 0; row < splitVect.size(); row++) {
		std::string line = splitVect[row];
		auto lSplitVect = Debug_StrSplit(line, " ");
		for (i32 col = 0; col < lSplitVect.size(); col++) {
			std::string pieceStr = lSplitVect[col];
			assert_exp(pieceStr.length() == 2);
			char pChar = pieceStr[0];
			char tChar = pieceStr[1];

			i32 playerId = CharToU32Digit(pChar);
			PieceType type = (tChar != '0') ? (PieceType)tChar : PieceType::None;

			Piece p = Piece(type, playerId);
			field[row][col].SetPiece(p);
		}
	}
#else
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
#endif
}

void ChessBoard::Debug_SetColorsForAttack(const std::vector<MovePos> &_av) {
	for (auto move : _av) {
		Square* s = &field[move.Row][move.Col];
		(*s).SetColor(SquareColor::Debug);
	}
}