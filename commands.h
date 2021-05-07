#if !defined(COMMANDS_H)
#define COMMANDS_H

#include "basic_types.h"
#include "piece.h"
#include "string"
#include "cutil.h"
#include "game_state.h"
#include "iostream"

class MoveCmd
{
private:
	std::ostream &out;
	std::istream &in;
	std::string userInput;
public:

	MoveCmd(std::ostream &out, std::istream &in);
	~MoveCmd();
	i32 PromptPlayerInput(u32 playerId, FieldPos &from, FieldPos &to, std::string &errMsg);
	std::string& GetUserInput();
};

#endif