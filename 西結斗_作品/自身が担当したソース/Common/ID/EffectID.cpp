#include "EffectID.h"
#include <unordered_map>
#include <assert.h>
#include "../Debug/Debug.h"

namespace {
	std::unordered_map<Effect_ID::EFFECT_ID, std::string> effectId;
}

void Effect_ID::DeleteEffectID()
{
	effectId.clear();
}

std::string Effect_ID::GetEffectID(EFFECT_ID _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)EFFECT_MAX) {
		Debug::CreateMessageBox("EffectHandleMiss", "miss");
	}
	return effectId[_type];
}

void Effect_ID::SetEffectID(std::string _effect, EFFECT_ID _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)EFFECT_MAX) {
		Debug::CreateMessageBox("EffectHandleMiss", "miss");
	}
	effectId[_type] = _effect;
}


Effect_ID::EFFECT_ID Effect_ID::StringToID(std::string _str)
{
	for (const auto& pair : effectId) {
		if (pair.second == _str) {
			return pair.first; // ílÇ™å©Ç¬Ç©Ç¡ÇΩÇÁÉLÅ[Çï‘Ç∑
		}
	}
	return EFFECT_ID::EFFECT_MAX;
}

