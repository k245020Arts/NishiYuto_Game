#include "StripTransitor.h"

StripTransitor::StripTransitor()
{
}

StripTransitor::~StripTransitor()
{
}

void StripTransitor::Update()
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

void StripTransitor::Draw()
{
	if (IsEnd()) {//èIóπÇµÇƒÇ¢ÇΩÇÁï‘Ç∑
		return;
	}
	const int width = 80;
	SetDrawScreen(DX_SCREEN_BACK);
	float rate = time / timeMax;	
	const int lp = (size.x / width) + 1;
	DrawRectGraph(0, 0, 0, 0, size.x, size.y, newWindow, true);
	for (int i = 0; i < lp; ++i) {
		if (i % 2 == 0) {
			DrawRectGraph(i * width, static_cast<int>(size.y * (rate - 1.0f)), i * width, 0, width, size.y, oldWindow, true);
		}
		else {
			DrawRectGraph(i * width, static_cast<int>(size.y * (rate - 1.0f)), i * width, 0, width, size.y, oldWindow, true);
		}
	}
}
