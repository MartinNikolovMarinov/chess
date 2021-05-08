#if !defined(SQUARE_H)
#define SQUARE_H

#include "basic_types.h"
#include "piece.h"
#include "display_buf.h"
#include "position.h"

enum SquareColor : u8 {
	Black = 0,
	White = 1,
	Debug = 2,
};

class Square : Displayer {
private:
	u32 width, height;
	SquareColor color;
	Piece piece;
	bool enPassant, hasOriginalPiece;
	FieldPos pos;
public:
	Square();
	Square(u32 width, u32 height, SquareColor color, const Piece *piece, FieldPos pos);
	~Square();

	Piece GetPiece();
	void SetPiece(const Piece *piece);
	SquareColor GetColor();
	void SetColor(SquareColor color);
	FieldPos GetPos();
	bool GetEnPassant();
	void SetEnPassant(bool value);
	void OriginalPieceMoved();
	bool HasOriginalPiece();

	void Display(DisplayBuffer *dbuf, u32 top, u32 left) override;
};

#endif