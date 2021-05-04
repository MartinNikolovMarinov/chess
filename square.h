#if !defined(SQUARE_H)
#define SQUARE_H

#include "basic_types.h"
#include "piece.h"
#include "display_buf.h"

enum SquareColor : u8 {
	Black = 0,
	White = 1,
};

class Square : Displayer {
private:
	u32 width, height;
	SquareColor color;
	Piece piece;
public:
	Square();
	Square(u32 width, u32 height, SquareColor color, Piece piece);
	~Square();

	void Display(DisplayBuffer &dbuf, u32 top, u32 left) override;
	void SetPiece(const Piece *piece);
	const Piece GetPiece() const;
};

#endif