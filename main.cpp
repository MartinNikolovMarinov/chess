#include <iostream>
#include <string>
#include <sstream>

#include "basic_types.h"

// n must be a number between 0 and 9
char u32DigitToChar(u32 n) {
	if (n > 9) {
		std::ostringstream msg;
		msg << "invalid value of argument n=" << n << std::endl;
		throw std::invalid_argument(msg.str());
	}
	return (char)n + 48;
}

enum SquareColor : u8 {
	Black = 0,
	White = 1,
};

class Square {
public:
	SquareColor color;
	u32 row;
	u32 col;
	std::string displayPos;

	Square();
	~Square();
	Square(SquareColor _c, u32 _row, u32 _col, std::string _dp);
};

Square::Square() {}
Square::~Square() {}
Square::Square(SquareColor _c, u32 _row, u32 _col, std::string _dp):
	color(_c), row(_row), col(_col), displayPos(_dp) {}

const u32 FIELD_SIZE = 8;

class ChessBoard {
public:
	Square field[FIELD_SIZE][FIELD_SIZE];

	ChessBoard();
	~ChessBoard();
};

ChessBoard::ChessBoard() {
	bool isWhite = true;
	for (i32 i = 0; i < FIELD_SIZE; i++) {
		const char displayRow = u32DigitToChar(FIELD_SIZE - i);
		for (i32 j = 0; j < FIELD_SIZE; j++) {
			SquareColor color = isWhite ? SquareColor::White : SquareColor::Black;
			const char displayCol = (char)j + 'A';
			std::string displayPos;
			displayPos.append(1, displayRow);
			displayPos.append(1, displayCol);
			field[i][j] = Square(color, i, j, displayPos);
			isWhite = !isWhite;
		}

		isWhite = !isWhite;
	}
}

ChessBoard::~ChessBoard() {}


int main() {
	// std::cout << "Hello World" << std::endl;
	// std::cout << "Hello World" << std::endl;
	// std::cout << "Hello World" << std::endl;

	std::cout << "" << std::endl;

	/*
		  h  g  f  e  d  c  b  a
		1
		2
		3
		4
		5
		6
		7
		8
	*/

	ChessBoard c;
	std::cout << &c;
	// std::cout << &s << std::endl;
	// std::cout << s.displayPosition << " " << &s.displayPosition << std::endl;
	// s = Square(SquareColor::Black, 1, 2, "test");
	// std::cout << &s << std::endl;
	// std::cout << s.displayPosition << " " << &s.displayPosition << std::endl;
}
