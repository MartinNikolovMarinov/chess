#if !defined(COMMANDS_H)
#define COMMANDS_H

#include "basic_types.h"
#include "piece.h"
#include "string"
#include "cutil.h"
#include "game_state.h"

i32 ParseMoveCommand(const std::string &input, MovePos &out, std::string &outErrMsg);
i32 PromptPlayerInput(GameState &gameState, MovePos &from, MovePos &to);

#endif