#include <stdint.h>

#if !defined(BASIC_TYPES_H)
#define BASIC_TYPES_H

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

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