#if !defined(TEXT_DISPLAY_H)
#define TEXT_DISPLAY_H

#include "basic_types.h"
#include "display_buf.h"

class TextDisplay : Displayer {
private:
	std::string msg;
public:
	TextDisplay();
	~TextDisplay();

	void SetMsg(std::string msg);
	void Display(DisplayBuffer &dbuf, u32 top, u32 left) override;
	void Clear();
};

#endif