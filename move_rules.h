#if !defined(MOVE_RULES_H)
#define MOVE_RULES_H

#include "piece.h"
#include "chess_board.h"

class MovementRules {
public:
	MovementRules();
	~MovementRules();

	void PushPieceLegalAttacks(ChessBoard &chessBoard, Piece &piece, MovePos &pos, std::vector<MovePos> &attackVect);
};

#endif