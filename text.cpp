#include "text.h"

TextDisplay::TextDisplay() {}
TextDisplay::~TextDisplay() {}
void TextDisplay::Clear() { msg.clear(); }
void TextDisplay::SetMsg(std::string _m) { msg = _m; }

void TextDisplay::Display(DisplayBuffer &_dbuf, u32 _top, u32 _left) {
	Displayer::Display(_dbuf, _top, _left);

	// Message will not fit, even by wrapping the text:
	u32 offsetRectArea = _left * _dbuf.GetHeight();
	u32 diplayRectArea = _dbuf.GetWidth() * _dbuf.GetHeight();
	assert_exp(offsetRectArea + msg.length() < diplayRectArea);

	u32 row = _top;
	u32 col = _left;
	for (i32 i = 0; i < msg.length(); i++, col++) {
		if (msg[i] == '\n') {
			// If the message has a new line, draw on the line below.
			row++;
			col = _left;
			// + 1 to skip the new line character:
			if (i + 1 >= msg.length()) { break; }
			i++;
		} else if (col >= _dbuf.GetWidth()) {
			// If the message can't fit into the buffer, draw it on the line below.
			row++;
			col = _left;
		}

		_dbuf.SetAt(row, col, msg[i]);
	}
}
