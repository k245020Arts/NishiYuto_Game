#include "FontID.h"
#include <unordered_map>
#include <assert.h>
#include "../Debug/Debug.h"

namespace {
	std::unordered_map<Font_ID::Font_ID, std::string> fontId;
}

void Font_ID::DeleteFontID()
{
	fontId.clear();
}

std::string Font_ID::GetFontID(Font_ID _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)Font_MAX) {
		Debug::CreateMessageBox("FontHandleMiss", "miss");
	}
	return fontId[_type];
}

void Font_ID::SetFontID(std::string _Font, Font_ID _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)Font_MAX) {
		Debug::CreateMessageBox("FontHandleMiss", "miss");
	}
	fontId[_type] = _Font;
}


Font_ID::Font_ID Font_ID::StringToID(std::string _str)
{
	for (const auto& pair : fontId) {
		if (pair.second == _str) {
			return pair.first; // ílÇ™å©Ç¬Ç©Ç¡ÇΩÇÁÉLÅ[Çï‘Ç∑
		}
	}
	return Font_ID::Font_MAX;
}

