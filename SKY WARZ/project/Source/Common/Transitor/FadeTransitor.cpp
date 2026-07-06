#include "FadeTransitor.h"

FadeTransitor::FadeTransitor()
{
	DontDestroyOnSceneChange();
}

FadeTransitor::~FadeTransitor()
{

}

void FadeTransitor::Update()
{
	switch (transitorType)
	{
	case Transitor::STAY:
		break;
	case Transitor::PLAY:
		time -= Time::DeltaTimeRate();
		if (time > 0) {
			
		}
		else {
			SetDrawScreen(newWindow);
			transitorType = FINISH;
		}
		break;
	case Transitor::FINISH:
		transitorType = STAY;
		SetDrawScreen(DX_SCREEN_BACK);
		break;
	default:
		break;
	}
	
}

void FadeTransitor::Draw()
{
	if (transitorType == FINISH) {
		return;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	float rate = time / timeMax;

	//切り替え先画面をアルファブレンディング
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(rate * 255));
	DrawGraph(0, 0, oldWindow, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}