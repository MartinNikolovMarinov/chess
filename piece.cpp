#include "piece.h"

Piece::Piece() : type(PieceType::None), playerId(-1) {}
Piece::Piece(PieceType _t, i32 _pid) : type(_t), playerId(_pid) {}
Piece::~Piece() {}
