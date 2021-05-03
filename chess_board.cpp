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
}
ChessBoard::~ChessBoard() {}

void ChessBoard::Display(DisplayBuffer *_dbuf, u32 _top, u32 _left) {
	assert_exp(_dbuf != nullptr);

	/* NOTE:
		If we just draw the squares right next to each other the boarders of the
		squares are drawn twice. The offsetForNPlusOne compensates for this, by
		drawing every (n+1) square one possition above where it should be.
		This removes padding between the above square.
	*/
	u32 offsetForNPlusOne = 0;
	for (i32 row = 0; row < FIELD_SIZE; row++) {
		u32 offRow;
		for (i32 col = 0; col < FIELD_SIZE; col++) {
			Square curr = field[row][col];
			// DEBUG:
			Piece p = Piece(PieceType::King, 1);
			curr.SetPiece(&p);
			// --DEBUG
			offRow = (row * SQUARE_HEIGHT) + _top - offsetForNPlusOne;
			u32 offCol = (col * SQUARE_WIDTH) + _left;
			curr.Display(_dbuf, offRow, offCol);
		}

		offsetForNPlusOne++;
	}
}
