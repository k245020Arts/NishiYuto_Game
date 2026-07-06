#include "SingleTonUpdater.h"
#include "../Sound/SoundManager.h"
#include "../Effect/EffectManager.h"
#include "../InputManager/InputManager.h"
#include "../../Stage/StageSelectData.h"
#include "../Debug/DebugLogText.h"

SingleTonUpdater::SingleTonUpdater()
{

}

SingleTonUpdater::~SingleTonUpdater()
{
}

void SingleTonUpdater::Update()
{
	SoundManager::GetInstance()->Update();
	EffectManager::GetInstance()->Update();
	InputManager::GetInstance()->Update();
	StageSelectData::GetInstance()->Update();
}

void SingleTonUpdater::Draw()
{
	SoundManager::GetInstance()->Draw();
	EffectManager::GetInstance()->Draw();
	StageSelectData::GetInstance()->Draw();
}

void SingleTonUpdater::Release()
{
	SoundManager::Destroy();
	EffectManager::Destroy();
	InputManager::Destroy();
	StageSelectData::Destroy();
	DebugLogText::Destroy();
}