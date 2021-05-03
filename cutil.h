#if !defined(CUTIL_H)
#define CUTIL_H

#include <iostream>
#include <sstream>

#include "basic_types.h"

// TODO: replace asserts with exceptions.
#define assert_exp(expression) if(!(expression)) { *(int *)0 = 0; }
char u32DigitToChar(u32 n);

#endif