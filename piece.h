#if !defined(PIECE_H)
#define PIECE_H

#include "basic_types.h"

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
	Piece(const Piece &p);
	Piece(PieceType type, i32 playerId);
	~Piece();

	PieceType GetType() const;
	void SetType(PieceType type);
	i32 GetPlayerId() const;
	void SetPlayerId(i32 playerId);
	bool GetEnPassant() const;
	void SetEnPassant(bool v);
	bool IsEmpty() const;
};

static const Piece EMPTY_PIECE;

#endif