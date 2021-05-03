#if !defined(CHESS_BOARD_H)
#define CHESS_BOARD_H

#include <iostream>

#include "basic_types.h"
#include "cutil.h"
#include "display_buf.h"
#include "piece.h"
#include "square.h"

const u32 FIELD_SIZE = 8;
const u32 SQUARE_WIDTH = 10;
const u32 SQUARE_HEIGHT = 5;
const u32 CHESSBOARD_WIDTH = FIELD_SIZE * SQUARE_WIDTH;
const u32 CHESSBOARD_HEIGHT = FIELD_SIZE * SQUARE_HEIGHT;

const u32 DISPLAY_WIDTH = CHESSBOARD_WIDTH + 10;
const u32 DISPLAY_HEIGHT = CHESSBOARD_HEIGHT;

class ChessBoard {
public:
	Square field[FIELD_SIZE][FIELD_SIZE];

	ChessBoard();
	~ChessBoard();

	void Display(DisplayBuffer *dbuf, u32 top, u32 left);
};

#endif