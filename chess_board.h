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
	bool canAttackSquare(u32 playerId, i32 attackedRow, i32 attackedCol);
public:

	ChessBoard();
	~ChessBoard();

	Piece GetPieceAt(i32 row, i32 col);
	Piece GetPieceAt(FieldPos pos);
	void SetPieceAt(i32 row, i32 col, const Piece *p);
	void SetPieceAt(FieldPos pos, const Piece *p);
	PieceType GetPieceTypeAt(i32 row, i32 col);
	PieceType GetPieceTypeAt(FieldPos pos);
	Square* GetSquareAt(i32 row, i32 col);
	Square* GetSquareAt(FieldPos pos);
	void GetPlayerSquares(u32 playerId, std::vector<Square*> &out);

	bool IsInRange(i32 row, i32 col);
	bool IsInRange(FieldPos _pos);
	bool IsOwnedByOpponent(u32 playerId, i32 row, i32 col);
	bool IsOwnedByOpponent(u32 playerId, FieldPos _pos);
	void SwapPieces(FieldPos first, FieldPos second);
	void PushIfAttackPossible(u32 playerId, i32 attackedRow, i32 attackedCol, std::vector<FieldPos> &attackVect);
	void CalcAttackVector(FieldPos from, FieldPos to, FieldPos direction, std::vector<FieldPos> &attackVect);

	void Display(DisplayBuffer *dbuf, u32 top, u32 left) override;
};

#endif