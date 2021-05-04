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

	GameState();
	GameState(ChessBoard &chessBoard);
	~GameState();

	void RotatePlayer();
	void Clear();
};

#endif