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
	std::vector<FieldPos> opponentAttackVect;
	std::string cmdInputLine;
	std::string errMsg;

	bool isGameOver = false;
	FieldPos currMovingFrom;
	FieldPos currMovingTo;

	GameState();
	GameState(ChessBoard &chessBoard, MovementRules &movementRules);
	~GameState();

	void RotatePlayer();
	Piece& GetFromPiece();
	Piece& GetToPiece();
	void Clear();
	i32 CheckBasicRules();

	void GetPlayerSquares(u32 playerId, std::vector<Square*> &out);
	void CalcOpponentAttackVect();
	bool IsCurrPlayerInCheck();
};

FieldPos FindKingSquare(const std::vector<Square*> _squares);

#endif