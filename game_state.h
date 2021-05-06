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

	// TODO: write pointless getters and setters for:
	u32 currPlayer = 1;
	std::vector<MovePos> currPlayerAttackVect;
	std::string inputLine; // from user
	std::string errMsg; // for user

	bool isGameOver = false;
	MovePos currMovingFrom;
	MovePos currMovingTo;

	GameState();
	GameState(ChessBoard &chessBoard, MovementRules &movementRules);
	~GameState();

	void RotatePlayer();
	Piece& GetFromPiece();
	Piece& GetToPiece();
	void Clear();
	i32 CheckBasicRules();

	void CalcCurrUserAttackVect();
};

#endif