#include "display_buf.h"
#include "chess_board.h"

int main() {
	// 1r, 1n, 1b, 1k, 1q, 1b, 1n, 1r
	// 1p, 1p, 1p, 1p, 1p, 1p, 1p, 1p

	DisplayBuffer dbuf = DisplayBuffer(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	dbuf.Clear('A');
	// Piece p = Piece(PieceType::King, 1);
	// Square s = Square(8, 5, SquareColor::Black, p);
	// s.Display(&dbuf, 5, 5);
	ChessBoard cb;
	cb.Display(&dbuf, 2, (DISPLAY_WIDTH - CHESSBOARD_WIDTH) / 2);

	dbuf.FlushTo(std::cout);
}
