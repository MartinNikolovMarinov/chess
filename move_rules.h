#if !defined(MOVE_RULES_H)
#define MOVE_RULES_H

#include "piece.h"
#include "chess_board.h"

class MovementRules {
public:
	MovementRules();
	~MovementRules();

	void PushPieceLegalAttacks(ChessBoard &chessBoard, Piece &piece, FieldPos &pos, std::vector<FieldPos> &attackVect);
};

#endif