#if !defined(GAME_STATE_H)
#define GAME_STATE_H

#include "chess_board.h"
#include "basic_types.h"

class GameState
{
public:
	ChessBoard *chessBoard;
	u32 currPlayer = 1;
	std::string inputLine;
	bool isDone = false;
	std::string errMsg;
	MovePos from;
	MovePos to;

	GameState();
	GameState(ChessBoard &chessBoard);
	~GameState();

	void RotatePlayer();
	const Piece& GetFromPiece();
	const Piece& GetToPiece();
	void Clear();
};

#endif