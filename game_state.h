#if !defined(GAME_STATE_H)
#define GAME_STATE_H

#include "chess_board.h"
#include "basic_types.h"
#include "move_rules.h"

class GameState
{
public:
	// IMPORTANT:
	// This kind of references should be shared pointers.
	// But the current implementation just keeps memory for them on the stack
	// and keeps them alive until the probgram ends.
	// Which means that referencing freed memory is not likely.
	ChessBoard &chessBoard;
	MovementRules &movementRules;

	u32 currPlayer = 1;
	std::vector<FieldPos> opponentAttackVect;
	std::string errMsg;

	// Special game states:
	bool isGameOver = false;

	FieldPos currMovingFrom;
	FieldPos currMovingTo;

	GameState();
	GameState(ChessBoard &chessBoard, MovementRules &movementRules);
	~GameState();

	void RotatePlayer();
	i32 CheckBasicRules();
	void Init();

	void GetPlayerSquares(u32 playerId, std::vector<Square*> &out);
	void CalcOpponentAttackVect();
	bool IsCurrPlayerInCheck();
	bool IsLegalCastlePos(const FieldPos &pos, u32 playerId);
	bool IsSquareUnderAttack(i32 row, i32 col);
	bool IsSquareUnderAttack(const FieldPos &p);
};

FieldPos FindKingSquare(const std::vector<Square*> squares);

#endif