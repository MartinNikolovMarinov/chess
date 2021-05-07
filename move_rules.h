#if !defined(MOVE_RULES_H)
#define MOVE_RULES_H

#include "piece.h"
#include "chess_board.h"

class MovementRules {
public:
	MovementRules();
	~MovementRules();

	void PushPieceLegalAttacks(ChessBoard &chessBoard, Piece &piece, FieldPos &pos, std::vector<FieldPos> &attackVect);
	void PushPieceLegalMoves(ChessBoard &_cb, Piece &_piece, FieldPos &_pos, std::vector<FieldPos> &_av);
};

#endif