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

struct MovePos { i32 Row, Col; };

static const MovePos UP_DIRECTION = {-1, 0};
static const MovePos DOWN_DIRECTION = {1, 0};
static const MovePos LEFT_DIRECTION = {0, -1};
static const MovePos RIGHT_DIRECTION = {0, 1};

static const MovePos UP_LEFT_DIRECTION = {-1, -1};
static const MovePos DOWN_LEFT_DIRECTION = {1, -1};
static const MovePos UP_RIGHT_DIRECTION = {-1, 1};
static const MovePos DOWN_RIGHT_DIRECTION = {1, 1};

#endif