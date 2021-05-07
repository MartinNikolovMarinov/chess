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
	Piece& GetPieceAt(const FieldPos& _pos);
	void SetPieceAt(i32 row, i32 col, const Piece &p);
	void SetPieceAt(const FieldPos& _pos, const Piece &_p);
	Square& GetSquareAt(i32 row, i32 col);

	bool IsInRange(i32 row, i32 col);
	bool IsInRange(const FieldPos& _pos);
	bool IsOwnedByOpponent(u32 playerId, i32 row, i32 col);
	bool IsOwnedByOpponent(u32 playerId, const FieldPos &_pos);
	void SwapPieces(const FieldPos first, const FieldPos second);
	bool CanAttackSquare(u32 playerId, i32 attackedRow, i32 attackedCol);
	void PushIfAttackPossible(u32 playerId, i32 attackedRow, i32 attackedCol, std::vector<FieldPos> &attackVect);
	void CalcAttackVector(const FieldPos &from, const FieldPos &to, const FieldPos &direction, std::vector<FieldPos> &attackVect);
	void Display(DisplayBuffer &dbuf, u32 top, u32 left) override;

	void Debug_SetColorsForAttack(const std::vector<FieldPos> &_av);
	void Debug_RemoveDebugColorsFromBoard();
};

#endif