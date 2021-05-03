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
public:
	PieceType type; // TODO: add useless getters and setters
	i32 playerId;

	Piece();
	Piece(PieceType type, i32 playerId);
	~Piece();
};

#endif