#if !defined(CUTIL_H)
#define CUTIL_H

#include <iostream>
#include <sstream>
#include <vector>

#include "basic_types.h"

// TODO: replace assert_exp with exceptions.
#define assert_exp(expression)             \
    if (!(expression)) {                   \
        std::cout << "Line: " << __LINE__; \
        std::cout << std::endl;            \
        std::cout << "File: " << __FILE__;  \
        std::cout << std::endl;            \
        *(int *)0 = 0;                     \
    }

char U32DigitToChar(u32 n);
u32 CharToU32Digit(char c);
u32 ChessDigitToCanonicalPos(char c);
u32 ChessLetterToCanonicalPos(char c);
char CanonicalPosToChessLetter(u32 n);
void ClearScreen();

std::vector<std::string> Debug_StrSplit(const std::string &s, const std::string &delim);

#endif