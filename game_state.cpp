#include "game_state.h"

GameState::GameState(ChessBoard &_cb) : chessBoard(&_cb) {}
GameState::~GameState() {}
void GameState::RotatePlayer() { currPlayer == 1 ? 2 : 1; }
void GameState::Clear() {
	inputLine.clear();
}