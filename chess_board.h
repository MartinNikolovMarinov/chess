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
const u32 DISPLAY_CHESS_CENTER_LEFT = (DISPLAY_WIDTH - CHESSBOARD_WIDTH) / 2;

class ChessBoard : Displayer {
private:
	Square field[FIELD_SIZE][FIELD_SIZE];

	void initBoardState();
public:

	ChessBoard();
	~ChessBoard();

	PieceType GetPieceTypeAt(i32 row, i32 col);
	Piece& GetPieceAt(i32 row, i32 col);
	void SetPieceAt(i32 _row, i32 _col, const Piece &p);
	bool IsInRange(i32 row, i32 col);
	void Display(DisplayBuffer &dbuf, u32 top, u32 left) override;
	bool CanAttackSquare(u32 playerId, i32 attackedRow, i32 attackedCol);
	void PushIfAttackPossible(u32 playerId, i32 attackedRow, i32 attackedCol, std::vector<MovePos> &attackVect);
	void CalcAttackVector(const MovePos &from, const MovePos &to, const MovePos &direction, std::vector<MovePos> &attackVect);

	void Debug_SetColorsForAttack(const std::vector<MovePos> &_av);
	void Debug_RemoveDebugColorsFromBoard();
};

#endif