#if !defined(GAME_STATE_H)
#define GAME_STATE_H

#include "chess_board.h"
#include "basic_types.h"
#include "move_rules.h"

class GameState
{
private:
	u32 currPlayer = 1;
public:
	// IMPORTANT:
	// This kind of pointers should be shared pointers.
	// But the current implementation just keeps memory for them on the stack
	// and keeps them alive until the probgram ends.
	// Which means that referencing freed memory is not likely.
	ChessBoard *chessBoard;
	MovementRules *movementRules;

	FieldPos currMovingFrom;
	FieldPos currMovingTo;
	// std::vector<Square*> currControlledSquares;
	// std::vector<Square*> opponentControlledSquares;
	std::string errMsg; // clear from api caller, not in Init !

	// Special game states:
	bool isGameOver = false;
	bool isInCheck = false;

	GameState();
	GameState(ChessBoard *chessBoard, MovementRules *movementRules);
	~GameState();
	u32 GetCurrPlayer();
	u32 GetOpponentPlayer();
	void RotatePlayer();
	void Init();

	bool CheckBasicRules();
	bool IsOpponentAttackingAt(i32 row, i32 col);
	bool IsOpponentAttackingAt(FieldPos p);
	bool IsCurrPlayerInCheck();
	bool IsCurrMoveLegal();
	bool IsLegalCastlePos(FieldPos pos, u32 playerId);
	bool TryMakeMove();
};

FieldPos FindKingSquare(const std::vector<Square*> squares);

#endif