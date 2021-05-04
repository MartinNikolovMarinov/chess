#include "game_state.h"

GameState::GameState(ChessBoard &_cb) : chessBoard(&_cb) {}
GameState::~GameState() {}
void GameState::RotatePlayer() { currPlayer == 1 ? 2 : 1; }
void GameState::Clear() {
	inputLine.clear();
}

const Piece& GameState::GetFromPiece() {
	const Piece& ret = chessBoard->field[from.Row][from.Col].GetPiece();
	return ret;
}

const Piece& GameState::GetToPiece() {
	const Piece& ret = chessBoard->field[to.Row][to.Col].GetPiece();
	return ret;
}
