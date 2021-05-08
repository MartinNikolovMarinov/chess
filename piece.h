#if !defined(PIECE_H)
#define PIECE_H

#include "basic_types.h"
#include "cutil.h"

enum PieceType : char {
	None = ' ',
	Pawn = 'P',
	Rook = 'R',
	Knight = 'N',
	Bishop = 'B',
	Queen = 'Q',
	King = 'K',
};

class Piece
{
private:
	PieceType type;
	i32 playerId;
public:

	Piece();
	Piece(const Piece *p);
	Piece(PieceType type, i32 playerId);
	~Piece();

	PieceType GetType();
	void SetType(PieceType type);
	i32 GetPlayerId();
	void SetPlayerId(i32 playerId);
	bool GetEnPassant();
	void SetEnPassant(bool v);
	bool IsEmpty();
};

static const Piece EMPTY_PIECE;

#endif