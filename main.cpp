#include "display_buf.h"
#include "chess_board.h"
#include "text.h"
#include "game_state.h"
#include "commands.h"

// TODO: all the code should be in a common namespace. (probably)
// TODO: should use map instead of a vector for attack positions!
// TODO: add all unnecessary getters and setters to comply to OOP nonsense.
// TODO: drop all constants everywhere.

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

bool CanAttackSquare(const ChessBoard &_cb, u32 _playerId, i32 _row, i32 _col) {
	if (_cb.IsInRange(_row, _col) == false) {
		return false;
	}
	const Piece &currPiece = _cb.GetPieceAt(_row, _col);
	bool currPieceIsNone = (currPiece.GetType() == PieceType::None);
	bool notOurPiece = currPiece.GetPlayerId() != _playerId;
	return (currPieceIsNone || notOurPiece);
}

void AddIfAttackPossible(const ChessBoard &_cb, u32 _playerId, i32 _row, i32 _col, std::vector<MovePos> &_attackVect) {
	bool canAttack = CanAttackSquare(_cb, _playerId, _row, _col);
	if (canAttack) {
		MovePos pos = {_row, _col};
		_attackVect.push_back(pos);
	}
}

void CalcAttackVector(const ChessBoard &_cb, // Function can be method of ChessBoard
	const MovePos &_from, const MovePos &_to, const MovePos &_direction,
	std::vector<MovePos> &_attackVect
) {
	const Piece &subjectPiece = _cb.GetPieceAt(_from.Row, _from.Col);
	assert_exp(subjectPiece.GetType() != PieceType::None);

	u32 playerId = subjectPiece.GetPlayerId();
	i32 currRow = _from.Row + _direction.Row;
	i32 currCol = _from.Col + _direction.Col;
	bool reachedDest = false;
	bool reachedOccupiedPos = false;
	while(!reachedDest && !reachedOccupiedPos) {
		AddIfAttackPossible(_cb, playerId, currRow, currCol, _attackVect);
		reachedOccupiedPos = _cb.GetPieceTypeAt(currRow, currCol) != PieceType::None;
		reachedDest = (currRow == _to.Row) && (currCol == _to.Col);
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

void AddKnightAttacks(const ChessBoard &_cb, const MovePos &_from, std::vector<MovePos> &_attackVect) {
	i32 currRow, currCol;
	bool canAttack;
	const Piece &subjectPiece = _cb.GetPieceAt(_from.Row, _from.Col);
	assert_exp(subjectPiece.GetType() != PieceType::None);
	u32 playerId = subjectPiece.GetPlayerId();

	AddIfAttackPossible(_cb, playerId, _from.Row - 2, _from.Col - 1, _attackVect);
	AddIfAttackPossible(_cb, playerId, _from.Row - 2, _from.Col + 1, _attackVect);
	AddIfAttackPossible(_cb, playerId, _from.Row - 1, _from.Col + 2, _attackVect);
	AddIfAttackPossible(_cb, playerId, _from.Row + 1, _from.Col + 2, _attackVect);
	AddIfAttackPossible(_cb, playerId, _from.Row + 2, _from.Col + 1, _attackVect);
	AddIfAttackPossible(_cb, playerId, _from.Row + 2, _from.Col - 1, _attackVect);
	AddIfAttackPossible(_cb, playerId, _from.Row + 1, _from.Col - 2, _attackVect);
	AddIfAttackPossible(_cb, playerId, _from.Row - 1, _from.Col - 2, _attackVect);
}

void AddKingAttacks(const ChessBoard &_cb, const MovePos &_from, std::vector<MovePos> &_attackVect) {
	bool canAttack;
	const Piece &subjectPiece = _cb.GetPieceAt(_from.Row, _from.Col);
	assert_exp(subjectPiece.GetType() != PieceType::None);
	u32 playerId = subjectPiece.GetPlayerId();

	AddIfAttackPossible(_cb, playerId, _from.Row + UP_DIRECTION.Row, _from.Col + UP_DIRECTION.Col, _attackVect);
	AddIfAttackPossible(_cb, playerId, _from.Row + DOWN_DIRECTION.Row, _from.Col + DOWN_DIRECTION.Col, _attackVect);
	AddIfAttackPossible(_cb, playerId, _from.Row + LEFT_DIRECTION.Row, _from.Col + LEFT_DIRECTION.Col, _attackVect);
	AddIfAttackPossible(_cb, playerId, _from.Row + RIGHT_DIRECTION.Row, _from.Col + RIGHT_DIRECTION.Col, _attackVect);
	AddIfAttackPossible(_cb, playerId, _from.Row + UP_LEFT_DIRECTION.Row, _from.Col + UP_LEFT_DIRECTION.Col, _attackVect);
	AddIfAttackPossible(_cb, playerId, _from.Row + DOWN_LEFT_DIRECTION.Row, _from.Col + DOWN_LEFT_DIRECTION.Col, _attackVect);
	AddIfAttackPossible(_cb, playerId, _from.Row + UP_RIGHT_DIRECTION.Row, _from.Col + UP_RIGHT_DIRECTION.Col, _attackVect);
	AddIfAttackPossible(_cb, playerId, _from.Row + DOWN_RIGHT_DIRECTION.Row, _from.Col + DOWN_RIGHT_DIRECTION.Col, _attackVect);
}

int main() {
	ChessBoard cb;
	GameState gm = GameState(cb);

	std::vector<MovePos> attackVect;
	MovePos from = {3, 3};
	AddKingAttacks(cb, from, attackVect);

	cb.Debug_SetColorsForAttack(attackVect);
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
