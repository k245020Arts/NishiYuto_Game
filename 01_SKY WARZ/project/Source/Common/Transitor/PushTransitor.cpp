#include "PushTransitor.h"

PushTransitor::PushTransitor()
{
	DontDestroyOnSceneChange();
	dir = UP;
}

PushTransitor::~PushTransitor()
{
}

void PushTransitor::Update()
{
	switch (transitorType)
	{
	case Transitor::STAY:
		break;
	case Transitor::PLAY:
		
		if (time == timeMax) {
			SetDrawScreen(DX_SCREEN_BACK);
		}
		time -= Time::DeltaTime();
		if (time <= 0.0f) {
			SetDrawScreen(newWindow);
			transitorType = FINISH;
		}
		else {

		}
		break;
	case Transitor::FINISH:
		SetDrawScreen(DX_SCREEN_BACK);
		transitorType = STAY;
		break;
	default:
		break;
	}
}

void PushTransitor::Draw()
{
	int bit;
	GetScreenState(&size.x, &size.y, &bit);
	if (IsEnd()) { //終了していたら返す
		return;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	float rate = 1.0f - (time / timeMax);

	//最終的に新画面が0に来るようにminusoneを用意する
	float minusone = rate - 1.0f;
	int endX = 0;
	int endY = 0;
	switch (dir) {
	case UP:
		endY = -size.y;
		break;
	case DOWN:
		endY = size.y;
		break;
	case LEFT:
		endX = -size.x;
		break;
	case RIGHT:
		endX = size.x;
		break;
	}

	DrawGraphF(endX * rate, endY * rate, oldWindow, true);
	DrawGraphF(endX * minusone, endY * minusone, newWindow, true);
	SetDrawScreen(DX_SCREEN_BACK);
}

void PushTransitor::StartPush(std::string _sceneName, float _time, Direction _dir)
{
	if (transitorType == PLAY) {
		return;
	}
	dir = _dir;
	Transitor::StartTransitor(_sceneName, _time);
}
