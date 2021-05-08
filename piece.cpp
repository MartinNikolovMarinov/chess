#include "piece.h"

Piece::Piece() : type(PieceType::None), playerId(0) {}
Piece::Piece(PieceType _t, i32 _pid) : type(_t), playerId(_pid) {}
Piece::Piece(const Piece *_p) {
    assert_exp(_p != nullptr);
    this->type = _p->type;
    this->playerId = _p->playerId;
}
Piece::~Piece() {}

PieceType Piece::GetType() { return type; }
void Piece::SetType(PieceType _t) { type = _t; }

i32 Piece::GetPlayerId() { return playerId; }
void Piece::SetPlayerId(i32 _pid) { playerId = _pid; }

bool Piece::IsEmpty() { return type == PieceType::None; }