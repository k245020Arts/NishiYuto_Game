#include "Sound.h"
#include "../../../Library/myDxLib.h"
#include "../../../Library/Time.h"
#include "../Easing.h"

namespace {
	static const int ParamMAX = 22050;
	float param[ParamMAX];
}

Sound::Sound()
{
	feedInTime		= 0.0f;
	feedOutTime		= 0.0f;
	active			= false;
	firstPlay		= false;
	loop			= false;
	volume			= 0;
	lastPlayintBgm	= 0;
	soundHandle		= -1;
	timeMax			= 0.0f;

}

Sound::~Sound()
{
	StopBGM();
}

void Sound::Update()
{
	if (feedInTime > 0.0f) {
		float valume = Easing::EasingFlow<float>(&feedInTime, timeMax, (float)volume, 0.0f, Easing::EaseIn<float>);
		ChangeVolumeSoundMem((int)valume, soundHandle);
	}
	if (feedOutTime > 0.0f) {
		float valume = Easing::EasingFlow<float>(&feedOutTime, timeMax, 0.0f, (float)volume, Easing::EaseIn<float>);
		ChangeVolumeSoundMem((int)valume, soundHandle);
	}
}

void Sound::SoundData(int _soundHandle, int _volume)
{
	soundHandle = _soundHandle;
	volume = _volume;
	lastPlayintBgm = _soundHandle;
	ChangeVolumeSoundMem(volume,soundHandle);
}

void Sound::PlaySe()
{
	PlaySoundMem(soundHandle, DX_PLAYTYPE_BACK);
	loop = false;
}

void Sound::PlayBGM(bool _loop, bool firstPlay)
{
	loop = _loop;
	if (!loop) {
		PlaySoundMem(soundHandle, DX_PLAYTYPE_BACK);
	}
	else {
		PlaySoundMem(soundHandle, DX_PLAYTYPE_LOOP,firstPlay);
	}
	
}


void Sound::StopBGM()
{
	StopSoundMem(soundHandle);
}

void Sound::StopSE()
{
	StopSoundMem(soundHandle);
}

void Sound::BGMFeedIn(float time)
{
	feedInTime	= time;
	timeMax		= time;
	active		= true;
}

void Sound::BGMFeedOut(float time)
{
	feedOutTime = time;
	timeMax		= time;
	active		= true;
}

void Sound::ChangeVolumeSound(int _volume)
{
	ChangeVolumeSoundMem(_volume, soundHandle);
}

void Sound::BaseChangeVolumeSound()
{
	ChangeVolumeSound(volume);
}

void Sound::PlayRamdomChangeFrequencySe(int _ramdom, int _baseFrequ)
{
	int ramdom = GetRand(_ramdom) + _baseFrequ;
	SetFrequencySoundMem(ramdom, soundHandle);
	PlaySoundMem(soundHandle, DX_PLAYTYPE_BACK);
}