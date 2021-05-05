#include "display_buf.h"
#include "chess_board.h"
#include "text.h"
#include "game_state.h"
#include "commands.h"

// TODO: all the code should be in a common namespace. (probably)
// TODO: should use map instead of a vector for attack positions!
// TODO: add all unnecessary getters and setters to comply to OOP nonsense.

i32 CheckBasicRules(GameState &_gm) {
	if (_gm.GetFromPiece().IsEmpty()) {
		_gm.errMsg = "There is not piece at that location.";
		return -1;
	}

	if (_gm.currPlayer != _gm.GetFromPiece().GetPlayerId()) {
		_gm.errMsg = "This piece does NOT belong to you.";
		return -1;
	}

	if (_gm.currPlayer == _gm.GetToPiece().GetPlayerId()) {
		_gm.errMsg = "You can NOT take your own piece.";
		return -1;
	}

	return 0;
}

void CalcAttackVector(const ChessBoard &_cb, // Function can be method of ChessBoard
	const MovePos &_from, const MovePos &_to, const MovePos &_direction,
	std::vector<MovePos> &_attackVect
) {
	const Piece &subectPiece = _cb.GetPieceAt(_from.Row, _from.Col);
	assert_exp(subectPiece.GetType() != PieceType::None);

	i32 currRow = _from.Row + _direction.Row;
	i32 currCol = _from.Col + _direction.Col;

	while(_cb.IsInRange(currRow, currCol)) {
		const Piece &currPiece = _cb.GetPieceAt(currRow, currCol);
		bool currPieceIsSet = (currPiece.GetType() != PieceType::None);
		bool reachedDest = (currRow == _to.Row) && (currCol == _to.Col);
		if (currPieceIsSet || reachedDest) {
			if (currPiece.GetPlayerId() != subectPiece.GetPlayerId()) {
				// If current piece is not owned by us, add it to the end result.
				MovePos pos = {currRow, currCol};
				_attackVect.push_back(pos);
			}
			break;
		}

		// Add to end result:
		MovePos pos = {currRow, currCol};
		_attackVect.push_back(pos);
		currRow += _direction.Row;
		currCol += _direction.Col;
	}
}

void AddRookAttacks(const ChessBoard &_cb, const MovePos &_from, std::vector<MovePos> &_attackVect) {
	MovePos to = {FIELD_SIZE - 1, _from.Col};
	CalcAttackVector(_cb, _from, to, RIGHT_DIRECTION, _attackVect);
	to = {_from.Row, FIELD_SIZE - 1};
	CalcAttackVector(_cb, _from, to, DOWN_DIRECTION, _attackVect);
	to = {0, _from.Col};
	CalcAttackVector(_cb, _from, to, UP_DIRECTION, _attackVect);
	to = {_from.Row, 0};
	CalcAttackVector(_cb, _from, to, LEFT_DIRECTION, _attackVect);
}

void AddBishopAttacks(const ChessBoard &_cb, const MovePos &_from, std::vector<MovePos> &_attackVect) {
	MovePos to = {0, 0};
	CalcAttackVector(_cb, _from, to, UP_LEFT_DIRECTION, _attackVect);
	to = {0, FIELD_SIZE - 1};
	CalcAttackVector(_cb, _from, to, UP_RIGHT_DIRECTION, _attackVect);
	to = {FIELD_SIZE - 1, 0};
	CalcAttackVector(_cb, _from, to, DOWN_LEFT_DIRECTION, _attackVect);
	to = {FIELD_SIZE - 1, FIELD_SIZE - 1};
	CalcAttackVector(_cb, _from, to, DOWN_RIGHT_DIRECTION, _attackVect);
}

void AddQueenAttacks(const ChessBoard &_cb, const MovePos &_from, std::vector<MovePos> &_attackVect) {
	AddRookAttacks(_cb, _from, _attackVect);
	AddBishopAttacks(_cb, _from, _attackVect);
}

int main() {
	ChessBoard cb;
	GameState gm = GameState(cb);

	std::vector<MovePos> attackMoves;
	MovePos from = {3, 3};
	AddQueenAttacks(cb, from, attackMoves);

	cb.Debug_SetColorsForAttack(attackMoves);
	DisplayBuffer dbuf = DisplayBuffer(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	ClearScreen();
	dbuf.Clear(' ');
	cb.Display(dbuf, 2, DISPLAY_CHESS_CENTER_LEFT);
	dbuf.FlushTo(std::cout);

	return 0;

	// DisplayBuffer dbuf = DisplayBuffer(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	// ChessBoard cb;
	// GameState gm = GameState(cb);
	// TextDisplay mfu = TextDisplay();

	// while(!gm.isDone) {
	// 	// Clean all state:
	// 	ClearScreen();
	// 	dbuf.Clear(' ');
	// 	gm.Clear();
	// 	mfu.Clear();

	// 	if (gm.errMsg.length() > 0) {
	// 		// Prev input from user had errors.
	// 		gm.errMsg += "\nPress enter to continue.";
	// 		mfu.SetMsg(gm.errMsg);
	// 		mfu.Display(dbuf, 15, 30);
	// 		dbuf.FlushTo(std::cout);
	// 		gm.errMsg.clear();

	// 		std::cin.get(); // TODO: first call to get reads the previous enter, for some reason.. can this be fixed?
	// 		while (std::cin.get() != '\n'); // wait for user to press space and ignore any other pressed key.
	// 		continue;
	// 	}

	// 	cb.Display(dbuf, 2, DISPLAY_CHESS_CENTER_LEFT);
	// 	dbuf.FlushTo(std::cout);

	// 	// Prompt the player for input. This blocks the thread
	// 	// so drawing needs to happen above this line!
	// 	gm.from = {};
	// 	gm.to = {};
	// 	if (PromptPlayerInput(gm, gm.from, gm.to) < 0) {
	// 		continue;
	// 	}

	// 	// Check move validity:
	// 	if (CheckBasicRules(gm) < 0) {
	// 		continue;
	// 	}

	// 	// Move the piece TMP DEBUG code:
	// 	const Piece fromP = gm.GetFromPiece();
	// 	// const Piece toP = gm.GetToPiece();
	// 	cb.field[gm.to.Row][gm.to.Col].SetPiece(fromP);
	// 	cb.field[gm.from.Row][gm.from.Col].SetPiece(EMPTY_PIECE);

	// 	gm.RotatePlayer();
	// }
}
