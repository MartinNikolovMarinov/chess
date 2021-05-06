#include "move_rules.h"

void pushRookAttacks(ChessBoard &_cb, MovePos &_from, std::vector<MovePos> &_av);
void pushBishopAttacks(ChessBoard &_cb, MovePos &_from, std::vector<MovePos> &_attackVect);
void pushQueenAttacks(ChessBoard &_cb, MovePos &_from, std::vector<MovePos> &_attackVect);
void pushKnightAttacks(ChessBoard &_cb, MovePos &_from, std::vector<MovePos> &_attackVect);
void pushKingAttacks(ChessBoard &_cb, MovePos &_from, std::vector<MovePos> &_attackVect);
void pushPawnkAttacks(ChessBoard &_cb, MovePos &_from, std::vector<MovePos> &_attackVect);

MovementRules::MovementRules() {}
MovementRules::~MovementRules() {}

void MovementRules::PushPieceLegalAttacks(ChessBoard &_cb, Piece &_piece, MovePos &_pos, std::vector<MovePos> &_av) {
	switch (_piece.GetType()) {
		case PieceType::Pawn:
			pushPawnkAttacks(_cb, _pos, _av);
			break;
		case PieceType::Rook:
			pushRookAttacks(_cb, _pos, _av);
			break;
		case PieceType::Knight:
			pushKnightAttacks(_cb, _pos, _av);
			break;
		case PieceType::Bishop:
			pushBishopAttacks(_cb, _pos, _av);
			break;
		case PieceType::Queen:
			pushQueenAttacks(_cb, _pos, _av);
			break;
		case PieceType::King:
			pushKingAttacks(_cb, _pos, _av);
			break;
		case PieceType::None:
			break;
		default:
			assert_exp(!"some piece has an invalid type!");
	}
}

void pushRookAttacks(ChessBoard &_cb, MovePos &_from, std::vector<MovePos> &_av) {
	MovePos to = {FIELD_SIZE - 1, _from.Col};
	_cb.CalcAttackVector(_from, to, RIGHT_DIRECTION, _av);
	to = {_from.Row, FIELD_SIZE - 1};
	_cb.CalcAttackVector(_from, to, DOWN_DIRECTION, _av);
	to = {0, _from.Col};
	_cb.CalcAttackVector(_from, to, UP_DIRECTION, _av);
	to = {_from.Row, 0};
	_cb.CalcAttackVector(_from, to, LEFT_DIRECTION, _av);
}

void pushBishopAttacks(ChessBoard &_cb, MovePos &_from, std::vector<MovePos> &_attackVect) {
	MovePos to = {0, 0};
	_cb.CalcAttackVector(_from, to, UP_LEFT_DIRECTION, _attackVect);
	to = {0, FIELD_SIZE - 1};
	_cb.CalcAttackVector(_from, to, UP_RIGHT_DIRECTION, _attackVect);
	to = {FIELD_SIZE - 1, 0};
	_cb.CalcAttackVector(_from, to, DOWN_LEFT_DIRECTION, _attackVect);
	to = {FIELD_SIZE - 1, FIELD_SIZE - 1};
	_cb.CalcAttackVector(_from, to, DOWN_RIGHT_DIRECTION, _attackVect);
}

void pushQueenAttacks(ChessBoard &_cb, MovePos &_from, std::vector<MovePos> &_attackVect) {
	pushRookAttacks(_cb, _from, _attackVect);
	pushBishopAttacks(_cb, _from, _attackVect);
}

void pushKnightAttacks(ChessBoard &_cb, MovePos &_from, std::vector<MovePos> &_attackVect) {
	i32 currRow, currCol;
	bool canAttack;
	Piece &subjectPiece = _cb.GetPieceAt(_from.Row, _from.Col);
	assert_exp(subjectPiece.GetType() != PieceType::None);
	u32 playerId = subjectPiece.GetPlayerId();

	_cb.PushIfAttackPossible(playerId, _from.Row - 2, _from.Col - 1, _attackVect);
	_cb.PushIfAttackPossible(playerId, _from.Row - 2, _from.Col + 1, _attackVect);
	_cb.PushIfAttackPossible(playerId, _from.Row - 1, _from.Col + 2, _attackVect);
	_cb.PushIfAttackPossible(playerId, _from.Row + 1, _from.Col + 2, _attackVect);
	_cb.PushIfAttackPossible(playerId, _from.Row + 2, _from.Col + 1, _attackVect);
	_cb.PushIfAttackPossible(playerId, _from.Row + 2, _from.Col - 1, _attackVect);
	_cb.PushIfAttackPossible(playerId, _from.Row + 1, _from.Col - 2, _attackVect);
	_cb.PushIfAttackPossible(playerId, _from.Row - 1, _from.Col - 2, _attackVect);
}

void pushKingAttacks(ChessBoard &_cb, MovePos &_from, std::vector<MovePos> &_attackVect) {
	bool canAttack;
	Piece &subjectPiece = _cb.GetPieceAt(_from.Row, _from.Col);
	assert_exp(subjectPiece.GetType() != PieceType::None);
	u32 playerId = subjectPiece.GetPlayerId();

	_cb.PushIfAttackPossible(playerId, _from.Row + UP_DIRECTION.Row, _from.Col + UP_DIRECTION.Col, _attackVect);
	_cb.PushIfAttackPossible(playerId, _from.Row + DOWN_DIRECTION.Row, _from.Col + DOWN_DIRECTION.Col, _attackVect);
	_cb.PushIfAttackPossible(playerId, _from.Row + LEFT_DIRECTION.Row, _from.Col + LEFT_DIRECTION.Col, _attackVect);
	_cb.PushIfAttackPossible(playerId, _from.Row + RIGHT_DIRECTION.Row, _from.Col + RIGHT_DIRECTION.Col, _attackVect);
	_cb.PushIfAttackPossible(playerId, _from.Row + UP_LEFT_DIRECTION.Row, _from.Col + UP_LEFT_DIRECTION.Col, _attackVect);
	_cb.PushIfAttackPossible(playerId, _from.Row + DOWN_LEFT_DIRECTION.Row, _from.Col + DOWN_LEFT_DIRECTION.Col, _attackVect);
	_cb.PushIfAttackPossible(playerId, _from.Row + UP_RIGHT_DIRECTION.Row, _from.Col + UP_RIGHT_DIRECTION.Col, _attackVect);
	_cb.PushIfAttackPossible(playerId, _from.Row + DOWN_RIGHT_DIRECTION.Row, _from.Col + DOWN_RIGHT_DIRECTION.Col, _attackVect);
}

void pushPawnkAttacks(ChessBoard &_cb, MovePos &_from, std::vector<MovePos> &_attackVect) {
	bool canAttack;
	Piece &subjectPiece = _cb.GetPieceAt(_from.Row, _from.Col);
	assert_exp(subjectPiece.GetType() != PieceType::None);
	u32 playerId = subjectPiece.GetPlayerId();

	if (playerId == 1) {
		_cb.PushIfAttackPossible(playerId, _from.Row + UP_LEFT_DIRECTION.Row, _from.Col + UP_LEFT_DIRECTION.Col, _attackVect);
		_cb.PushIfAttackPossible(playerId, _from.Row + UP_RIGHT_DIRECTION.Row, _from.Col + UP_RIGHT_DIRECTION.Col, _attackVect);
	} else {
		_cb.PushIfAttackPossible(playerId, _from.Row + DOWN_LEFT_DIRECTION.Row, _from.Col + DOWN_LEFT_DIRECTION.Col, _attackVect);
		_cb.PushIfAttackPossible(playerId, _from.Row + DOWN_RIGHT_DIRECTION.Row, _from.Col + DOWN_RIGHT_DIRECTION.Col, _attackVect);
	}
}