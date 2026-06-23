#include "SoundID.h"
#include <unordered_map>
#include <assert.h>
#include "../Debug/Debug.h"

namespace {

	std::unordered_map<Sound_ID::SOUND_ID, std::string>* soundId;
}

void Sound_ID::InitID()
{
	soundId = new std::unordered_map<Sound_ID::SOUND_ID, std::string>;
}

void Sound_ID::DeleteSoundID()
{
	soundId->clear();
	delete soundId;
}

std::string Sound_ID::GetSoundID(SOUND_ID _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)SOUND_MAX) {
		Debug::CreateMessageBox("soundHandleMiss", "miss");
	}
	return (*soundId)[_type];
}

void Sound_ID::SetSoundId(std::string _sound, SOUND_ID _type)
{
	int type = (int)_type;
	if (type < 0 && type >= (int)SOUND_MAX) {
		Debug::CreateMessageBox("soundHandleSetMiss", "miss");
	}
	(*soundId)[_type] = _sound;
}


Sound_ID::SOUND_ID Sound_ID::StringToID(std::string _str)
{
	for (const auto& pair : *soundId) {
		if (pair.second == _str) {
			return pair.first; // ’l‚ªŒ©‚Â‚©‚Á‚½‚çƒL[‚ğ•Ô‚·
		}
	}
	return Sound_ID::SOUND_MAX;
}

