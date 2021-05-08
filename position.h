#if !defined(POSITION_H)
#define POSITION_H

#include "basic_types.h"

struct FieldPos {
    i32 Row, Col;

    FieldPos operator+(const FieldPos rhs) const {
        FieldPos res = { Row + rhs.Row, Col + rhs.Col };
        return res;
    }
    FieldPos operator+=(const FieldPos rhs) {
        Row += rhs.Row;
        Col += rhs.Col;
        return *this;
    }
    FieldPos operator-(const FieldPos rhs) const {
        FieldPos res = { Row - rhs.Row, Col - rhs.Col };
        return res;
    }
    FieldPos operator-=(const FieldPos rhs) {
        Row -= rhs.Row;
        Col -= rhs.Col;
        return *this;
    }
    bool operator==(const FieldPos rhs) const {
        return (Row == rhs.Row && Col == rhs.Col);
    }
    bool operator!=(const FieldPos rhs) const {
        return (Row != rhs.Row || Col != rhs.Col);
    }
};

FieldPos ToDirectionVect(FieldPos pos);

static const FieldPos ZERO_POS = {0, 0};
static const FieldPos UP_DIRECTION = {-1, 0};
static const FieldPos DOWN_DIRECTION = {1, 0};
static const FieldPos LEFT_DIRECTION = {0, -1};
static const FieldPos RIGHT_DIRECTION = {0, 1};
static const FieldPos UP_LEFT_DIRECTION = {-1, -1};
static const FieldPos DOWN_LEFT_DIRECTION = {1, -1};
static const FieldPos UP_RIGHT_DIRECTION = {-1, 1};
static const FieldPos DOWN_RIGHT_DIRECTION = {1, 1};

static const FieldPos BOTTOM_LEFT_CASTLE_POS = {7, 2};
static const FieldPos BOTTOM_RIGHT_CASTLE_POS = {7, 6};
static const FieldPos TOP_LEFT_CASTLE_POS = {0, 2};
static const FieldPos TOP_RIGHT_CASTLE_POS = {0, 6};

#endif