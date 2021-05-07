#include "cutil.h"

// n must be a number between 0 and 9
char U32DigitToChar(u32 _n) {
	if (_n > 9) {
		assert_exp(!"invalid value of argument");
	}
	return (char)_n + 48;
}

// n must be a number between 0 and 9
u32 CharToU32Digit(char _c) {
	if ('0' > _c || _c > '9') {
		assert_exp(!"invalid value of argument");
	}
	return (u32)(_c - 48);
}

void ClearScreen() {
	#ifdef WINDOWS
		std::system("cls");
	#else
		// Assume POSIX
		std::system ("clear");
	#endif
}

u32 ChessDigitToCanonicalPos(char _c) {
	switch (_c) {
		case '8': return 0;
		case '7': return 1;
		case '6': return 2;
		case '5': return 3;
		case '4': return 4;
		case '3': return 5;
		case '2': return 6;
		case '1': return 7;
	}
	assert_exp(!"invalid value of argument")
	return 0;
}

u32 ChessLetterToCanonicalPos(char _c) {
	switch (_c) {
		case 'A': return 0;
		case 'B': return 1;
		case 'C': return 2;
		case 'D': return 3;
		case 'E': return 4;
		case 'F': return 5;
		case 'G': return 6;
		case 'H': return 7;
	}
	assert_exp(!"invalid value of argument")
	return 0;
}

char CanonicalPosToChessLetter(u32 _n) {
	switch (_n) {
		case 0: return 'A';
		case 1: return 'B';
		case 2: return 'C';
		case 3: return 'D';
		case 4: return 'E';
		case 5: return 'F';
		case 6: return 'G';
		case 7: return 'H';
	}
	assert_exp(!"invalid value of argument")
	return 0;
}

std::vector<std::string> Debug_StrSplit(const std::string &_s, const std::string &_delim) {
	std::size_t start = 0U;
    std::size_t end = _s.find(_delim);
	std::vector<std::string> res;
    while (end != std::string::npos)
    {
        res.push_back(_s.substr(start, end - start));
        start = end + _delim.length();
        end = _s.find(_delim, start);
    }

	res.push_back(_s.substr(start, end));
	return res;
}