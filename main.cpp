#include "display_buf.h"
#include "chess_board.h"

// TODO: all the code should be in a common namespace. (probably)
// TODO: display function is common for all visible/window elements.

struct Command {
	Piece p;
	u32 fromRow, fromCol, toRow, toCol;
};

struct CmdPos {
	u32 row;
	u32 col;
};

i32 ParseMoveCommand(const std::string& _input, CmdPos *_out, std::string& _errMsg) {
	if (_input.length() != 2) {
		_errMsg = "Invalid input format. Try A1 for example.";
		return -1;
	};

	char rowChar = _input[0];
	char colChar = _input[1];
	i32 row = -1, col = -1;
	if ('1' <= rowChar && rowChar <= '9') row = ChessDigitToCanonicalPos(rowChar);
	if ('A' <= colChar && colChar <= 'H') col = ChessLetterToCanonicalPos(colChar);

	if (row == -1) {
		_errMsg = "Invalid Row.";
		return -1;
	}
	if (col == -1) {
		_errMsg = "Invalid Col.";
		return -1;
	};

	_out->row = (u32)row;
	_out->col = (u32)col;
	return 0;
}

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

GameState::GameState(ChessBoard &_cb) : chessBoard(&_cb) {}
GameState::~GameState() {}
void GameState::RotatePlayer() { currPlayer == 1 ? 2 : 1; }
void GameState::Clear() {
	inputLine.clear();
}

class MsgForUser
{
private:
	std::string msg;
public:
	MsgForUser();
	~MsgForUser();

	void SetMsg(std::string msg);
	void Display(DisplayBuffer &dbuf, u32 top, u32 left);
	void Clear();
};

MsgForUser::MsgForUser() {}
MsgForUser::~MsgForUser() {}
void MsgForUser::Clear() { msg.clear(); }
void MsgForUser::SetMsg(std::string _m) { msg = _m; }

void MsgForUser::Display(DisplayBuffer &_dbuf, u32 _top, u32 _left) {
	// TODO: use common asserts from virtual method.

	// Message will not fit, even by wrapping the text:
	u32 offsetRectArea = _left * _dbuf.GetHeight();
	u32 diplayRectArea = _dbuf.GetWidth() * _dbuf.GetHeight();
	assert_exp(offsetRectArea + msg.length() < diplayRectArea);

	u32 row = _top;
	u32 col = _left;
	for (i32 i = 0; i < msg.length(); i++, col++) {
		if (msg[i] == '\n') {
			// If the message has a new line, draw on the line below.
			row++;
			col = _left;
			// + 1 to skip the new line character:
			if (i + 1 >= msg.length()) { break; }
			i++;
		} else if (col >= _dbuf.GetWidth()) {
			// If the message can't fit into the buffer, draw it on the line below.
			row++;
			col = _left;
		}

		_dbuf.SetAt(row, col, msg[i]);
	}
}

int main() {
    DisplayBuffer dbuf = DisplayBuffer(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	ChessBoard cb;
	GameState gm = GameState(cb);
	MsgForUser mfu = MsgForUser();

	char c = '1';
	while(!gm.isDone) {
		// Clean all state:
		ClearScreen();
		dbuf.Clear(' ');
		gm.Clear();
		mfu.Clear();

		if (gm.errMsg.length() > 0) {
			// Prev input from user had errors.
			gm.errMsg += "\nPress enter to continue.";
			mfu.SetMsg(gm.errMsg);
			mfu.Display(dbuf, 15, 30);
			dbuf.FlushTo(std::cout);
			gm.errMsg.clear();

			std::cin.get(); // TODO: first get reads the previous enter, for some reason.. can this be fixed?
			while (std::cin.get() != '\n'); // wait for user to press space and ignore any other pressed key.
			continue;
		}

		// DEBUG: from this line down:
		cb.Display(&dbuf, 2, DISPLAY_CHESS_CENTER_LEFT);
		dbuf.FlushTo(std::cout);

		std::cout << "Player " << gm.currPlayer << " make a move!" <<  std::endl;
		std::cout << "Type row first then col, for example 1A is a valid location." <<  std::endl;
		std::cout << "Type the location of the peace you want to move: ";
		std::cin >> gm.inputLine;
		CmdPos from = {};
		i32 res = ParseMoveCommand(gm.inputLine, &from, gm.errMsg);
		if (res < 0) {
			continue;
		}

		std::cout << std::endl;
		std::cout << "Type the location you want to move it: ";
		std::cin >> gm.inputLine;
		CmdPos to = {};
		res = ParseMoveCommand(gm.inputLine, &to, gm.errMsg);
		if (res < 0) {
			continue;
		}

		const Piece fromP = cb.field[from.row][from.col].GetPiece();
		const Piece toP = cb.field[to.row][to.col].GetPiece();

		cb.field[to.row][to.col].SetPiece(&fromP);
		cb.field[from.row][from.col].SetPiece(&toP);

		gm.RotatePlayer();
	}
}
