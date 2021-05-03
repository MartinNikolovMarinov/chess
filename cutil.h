#if !defined(CUTIL_H)
#define CUTIL_H

#include <iostream>
#include <sstream>

#include "basic_types.h"

// TODO: replace assert_exp with exceptions.
#define assert_exp(expression) if(!(expression)) { *(int *)0 = 0; }

char U32DigitToChar(u32 n);
u32 ChessDigitToCanonicalPos(char _c);
u32 ChessLetterToCanonicalPos(char _c);
char CanonicalPosToChessLetter(u32 _n);
void ClearScreen();

#endif