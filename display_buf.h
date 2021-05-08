#if !defined(DISPLAY_BUF_H)
#define DISPLAY_BUF_H

#include <iostream>
#include "basic_types.h"
#include "cutil.h"

class DisplayBuffer {
private:
	u32 width, height;
	char *renderBuf;

	bool isInRange(u32 row, u32 col);
public:
	DisplayBuffer(u32 width, u32 height);
	~DisplayBuffer();

	u32 GetWidth();
	u32 GetHeight();
	char GetAt(u32 row, u32 col);
	void SetAt(u32 row, u32 col, char value);
    void Clear(char v = 0);
	void FlushTo(std::ostream& outputStream);
};

class Displayer {
public:
    virtual void Display(DisplayBuffer *_dbuf, u32 _top, u32 _left) {
		assert_exp(_dbuf != nullptr);
		assert_exp(_top < _dbuf->GetHeight());
		assert_exp(_left < _dbuf->GetWidth());
    }
};

#endif