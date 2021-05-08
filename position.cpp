#include "position.h"

FieldPos ToDirectionVect(FieldPos pos) {
    FieldPos ret;

    if (pos.Row > 0) ret.Row = 1;
    else if ((pos.Row < 0)) ret.Row = -1;
    else ret.Row = 0;

    if (pos.Col > 0) ret.Col = 1;
    else if ((pos.Col < 0)) ret.Col = -1;
    else ret.Col = 0;

    return ret;
}