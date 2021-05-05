#include "game_state.h"

GameState::GameState(ChessBoard &_cb) : chessBoard(&_cb) {}
GameState::~GameState() {}
void GameState::RotatePlayer() { currPlayer == 1 ? 2 : 1; }
void GameState::Clear() {
	inputLine.clear();
}

const Piece& GameState::GetFromPiece() const {
	const Piece& ret = chessBoard->GetPieceAt(from.Row, from.Col);
	return ret;
}

const Piece& GameState::GetToPiece() const {
	const Piece& ret = chessBoard->GetPieceAt(to.Row, to.Col);
	return ret;
}
