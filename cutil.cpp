#include "cutil.h"

// n must be a number between 0 and 9
char u32DigitToChar(u32 n) {
	if (n > 9) {
		std::ostringstream msg;
		msg << "invalid value of argument n=" << n << std::endl;
		throw std::invalid_argument(msg.str());
	}
	return (char)n + 48;
}

