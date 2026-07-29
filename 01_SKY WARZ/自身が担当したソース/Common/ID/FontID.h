#pragma once
#include <string>

namespace Font_ID {

	enum Font_ID
	{
		TITLE_USE_FONT,
		PAUSE_FONT,
		UI_FONT,
		Font_MAX
	};

	void DeleteFontID();

	const std::string GetFontID(Font_ID _type);

	void SetFontID(std::string _Font, Font_ID _type);

	Font_ID StringToID(std::string _str);
}