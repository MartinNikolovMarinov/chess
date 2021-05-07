#include "move_rules.h"

void pushRookAttacks(ChessBoard &chessBoard, FieldPos &from, std::vector<FieldPos> &attackVect);
void pushBishopAttacks(ChessBoard &chessBoard, FieldPos &from, std::vector<FieldPos> &attackVect);
void pushQueenAttacks(ChessBoard &chessBoard, FieldPos &from, std::vector<FieldPos> &attackVect);
void pushKnightAttacks(ChessBoard &chessBoard, FieldPos &from, std::vector<FieldPos> &attackVect);
void pushKingAttacks(ChessBoard &chessBoard, FieldPos &from, std::vector<FieldPos> &attackVect);
void pushKingMoves(ChessBoard &chessBoard, FieldPos &from, std::vector<FieldPos> &attackVect);
void pushPawnkAttacks(ChessBoard &chessBoard, FieldPos &from, std::vector<FieldPos> &attackVect);
void pushPawnMoves(ChessBoard &chessBoard, FieldPos &from, std::vector<FieldPos> &attackVect);

MovementRules::MovementRules() {}
MovementRules::~MovementRules() {}

void MovementRules::PushPieceLegalAttacks(ChessBoard &_cb, Piece &_piece, FieldPos &_pos, std::vector<FieldPos> &_av) {
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

void MovementRules::PushPieceLegalMoves(ChessBoard &_cb, Piece &_piece, FieldPos &_pos, std::vector<FieldPos> &_av) {
	switch (_piece.GetType()) {
		case PieceType::Pawn:
			pushPawnMoves(_cb, _pos, _av);
			break;
		case PieceType::King:
			pushKingMoves(_cb, _pos, _av);
			break;
		case PieceType::Knight:
		case PieceType::Rook:
		case PieceType::Bishop:
		case PieceType::Queen:
		case PieceType::None:
			this->PushPieceLegalAttacks(_cb, _piece, _pos, _av);
			break;
		default:
			assert_exp(!"some piece has an invalid type!");
	}
}

void pushRookAttacks(ChessBoard &_cb, FieldPos &_from, std::vector<FieldPos> &_av) {
	FieldPos to = {FIELD_SIZE - 1, _from.Col};
	_cb.CalcAttackVector(_from, to, RIGHT_DIRECTION, _av);
	to = {_from.Row, FIELD_SIZE - 1};
	_cb.CalcAttackVector(_from, to, DOWN_DIRECTION, _av);
	to = {0, _from.Col};
	_cb.CalcAttackVector(_from, to, UP_DIRECTION, _av);
	to = {_from.Row, 0};
	_cb.CalcAttackVector(_from, to, LEFT_DIRECTION, _av);
}

void pushBishopAttacks(ChessBoard &_cb, FieldPos &_from, std::vector<FieldPos> &_av) {
	FieldPos to = {0, 0};
	_cb.CalcAttackVector(_from, to, UP_LEFT_DIRECTION, _av);
	to = {0, FIELD_SIZE - 1};
	_cb.CalcAttackVector(_from, to, UP_RIGHT_DIRECTION, _av);
	to = {FIELD_SIZE - 1, 0};
	_cb.CalcAttackVector(_from, to, DOWN_LEFT_DIRECTION, _av);
	to = {FIELD_SIZE - 1, FIELD_SIZE - 1};
	_cb.CalcAttackVector(_from, to, DOWN_RIGHT_DIRECTION, _av);
}

void pushQueenAttacks(ChessBoard &_cb, FieldPos &_from, std::vector<FieldPos> &_av) {
	pushRookAttacks(_cb, _from, _av);
	pushBishopAttacks(_cb, _from, _av);
}

void pushKnightAttacks(ChessBoard &_cb, FieldPos &_from, std::vector<FieldPos> &_av) {
	i32 currRow, currCol;
	bool canAttack;
	Piece &subjectPiece = _cb.GetPieceAt(_from.Row, _from.Col);
	assert_exp(subjectPiece.GetType() != PieceType::None);
	u32 playerId = subjectPiece.GetPlayerId();

	_cb.PushIfAttackPossible(playerId, _from.Row - 2, _from.Col - 1, _av);
	_cb.PushIfAttackPossible(playerId, _from.Row - 2, _from.Col + 1, _av);
	_cb.PushIfAttackPossible(playerId, _from.Row - 1, _from.Col + 2, _av);
	_cb.PushIfAttackPossible(playerId, _from.Row + 1, _from.Col + 2, _av);
	_cb.PushIfAttackPossible(playerId, _from.Row + 2, _from.Col + 1, _av);
	_cb.PushIfAttackPossible(playerId, _from.Row + 2, _from.Col - 1, _av);
	_cb.PushIfAttackPossible(playerId, _from.Row + 1, _from.Col - 2, _av);
	_cb.PushIfAttackPossible(playerId, _from.Row - 1, _from.Col - 2, _av);
}

void pushKingAttacks(ChessBoard &_cb, FieldPos &_from, std::vector<FieldPos> &_av) {
	bool canAttack;
	Piece &subjectPiece = _cb.GetPieceAt(_from.Row, _from.Col);
	assert_exp(subjectPiece.GetType() != PieceType::None);
	u32 playerId = subjectPiece.GetPlayerId();

	_cb.PushIfAttackPossible(playerId, _from.Row + UP_DIRECTION.Row, _from.Col + UP_DIRECTION.Col, _av);
	_cb.PushIfAttackPossible(playerId, _from.Row + DOWN_DIRECTION.Row, _from.Col + DOWN_DIRECTION.Col, _av);
	_cb.PushIfAttackPossible(playerId, _from.Row + LEFT_DIRECTION.Row, _from.Col + LEFT_DIRECTION.Col, _av);
	_cb.PushIfAttackPossible(playerId, _from.Row + RIGHT_DIRECTION.Row, _from.Col + RIGHT_DIRECTION.Col, _av);
	_cb.PushIfAttackPossible(playerId, _from.Row + UP_LEFT_DIRECTION.Row, _from.Col + UP_LEFT_DIRECTION.Col, _av);
	_cb.PushIfAttackPossible(playerId, _from.Row + DOWN_LEFT_DIRECTION.Row, _from.Col + DOWN_LEFT_DIRECTION.Col, _av);
	_cb.PushIfAttackPossible(playerId, _from.Row + UP_RIGHT_DIRECTION.Row, _from.Col + UP_RIGHT_DIRECTION.Col, _av);
	_cb.PushIfAttackPossible(playerId, _from.Row + DOWN_RIGHT_DIRECTION.Row, _from.Col + DOWN_RIGHT_DIRECTION.Col, _av);
}

void pushKingMoves(ChessBoard &_cb, FieldPos &_from, std::vector<FieldPos> &_av) {
	pushKingAttacks(_cb, _from, _av);
}

void pushPawnkAttacks(ChessBoard &_cb, FieldPos &_from, std::vector<FieldPos> &_av) {
	bool canAttack;
	Piece &subjectPiece = _cb.GetPieceAt(_from.Row, _from.Col);
	assert_exp(subjectPiece.GetType() != PieceType::None);
	u32 playerId = subjectPiece.GetPlayerId();

	if (playerId == 1) {
		_cb.PushIfAttackPossible(playerId, _from.Row + UP_LEFT_DIRECTION.Row, _from.Col + UP_LEFT_DIRECTION.Col, _av);
		_cb.PushIfAttackPossible(playerId, _from.Row + UP_RIGHT_DIRECTION.Row, _from.Col + UP_RIGHT_DIRECTION.Col, _av);
	} else {
		_cb.PushIfAttackPossible(playerId, _from.Row + DOWN_LEFT_DIRECTION.Row, _from.Col + DOWN_LEFT_DIRECTION.Col, _av);
		_cb.PushIfAttackPossible(playerId, _from.Row + DOWN_RIGHT_DIRECTION.Row, _from.Col + DOWN_RIGHT_DIRECTION.Col, _av);
	}
}

void pushPawnMoves(ChessBoard &_cb, FieldPos &_from, std::vector<FieldPos> &_av) {
	bool canAttack;
	Piece &subjectPiece = _cb.GetPieceAt(_from.Row, _from.Col);
	assert_exp(subjectPiece.GetType() != PieceType::None);
	u32 playerId = subjectPiece.GetPlayerId();

	if (playerId == 1) {
		if (_from.Row == 6) {
			// Allow 2 square moves if the pawn is on its starting position.
			_cb.PushIfAttackPossible(playerId, (_from.Row + UP_DIRECTION.Row) - 1, _from.Col + UP_DIRECTION.Col, _av);
		}
		_cb.PushIfAttackPossible(playerId, _from.Row + UP_DIRECTION.Row, _from.Col + UP_DIRECTION.Col, _av);
		if (_cb.IsOwnedByOpponent(playerId, _from + UP_LEFT_DIRECTION)) {
			_cb.PushIfAttackPossible(playerId, _from.Row + UP_LEFT_DIRECTION.Row, _from.Col + UP_LEFT_DIRECTION.Col, _av);
		}
		if (_cb.IsOwnedByOpponent(playerId, _from + UP_RIGHT_DIRECTION)) {
			_cb.PushIfAttackPossible(playerId, _from.Row + UP_RIGHT_DIRECTION.Row, _from.Col + UP_RIGHT_DIRECTION.Col, _av);
		}
	} else {
		if (_from.Row == 1) {
			// Allow 2 square moves if the pawn is on its starting position.
			_cb.PushIfAttackPossible(playerId, (_from.Row + DOWN_DIRECTION.Row) + 1, _from.Col + UP_DIRECTION.Col, _av);
		}
		_cb.PushIfAttackPossible(playerId, _from.Row + DOWN_DIRECTION.Row, _from.Col + DOWN_DIRECTION.Col, _av);
		if (_cb.IsOwnedByOpponent(playerId, _from + DOWN_LEFT_DIRECTION)) {
			_cb.PushIfAttackPossible(playerId, _from.Row + DOWN_LEFT_DIRECTION.Row, _from.Col + DOWN_LEFT_DIRECTION.Col, _av);
		}
		if (_cb.IsOwnedByOpponent(playerId, _from + DOWN_RIGHT_DIRECTION)) {
			_cb.PushIfAttackPossible(playerId, _from.Row + DOWN_RIGHT_DIRECTION.Row, _from.Col + DOWN_RIGHT_DIRECTION.Col, _av);
		}
	}
}