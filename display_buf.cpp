#include "display_buf.h"

DisplayBuffer::DisplayBuffer(u32 _w, u32 _h): width(_w), height(_h) {
	renderBuf = new char[_w * _h](); // the () inits values to 0, how stupid is that syntax ...
}
DisplayBuffer::~DisplayBuffer() {
	delete renderBuf;
}
u32 DisplayBuffer::GetWidth() { return width; }
u32 DisplayBuffer::GetHeight() { return height; }

bool DisplayBuffer::isInRange(u32 _row, u32 _col) {
	return _row < height && _col < width;
}
char DisplayBuffer::GetAt(u32 _row, u32 _col) {
	assert_exp(isInRange(_row, _col));
	return renderBuf[(_row * width) + _col];
}
void DisplayBuffer::SetAt(u32 _row, u32 _col, char _v) {
	assert_exp(isInRange(_row, _col));
	renderBuf[(_row * width) + _col] = _v;
}

void DisplayBuffer::Clear(char _v) {
    for (i32 row = 0; row < height; row++) {
		for (i32 col = 0; col < width; col++) {
			SetAt(row, col, _v);
		}
	}
}

void DisplayBuffer::FlushTo(std::ostream& _buf) {
	for (i32 row = 0; row < height; row++) {
		for (i32 col = 0; col < width; col++) {
			_buf << GetAt(row, col);
		}
		_buf << std::endl;
	}
}