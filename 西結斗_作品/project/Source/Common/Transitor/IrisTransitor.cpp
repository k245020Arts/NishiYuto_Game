#include "IrisTransitor.h"
#include <math.h>
#include "../Debug/Debug.h"

IrisTransitor::IrisTransitor()
{
	maskHandle	= -1;
	maskScreen	= -1;
	sizeCross	= -1;
	irisOut		= false;
}

IrisTransitor::~IrisTransitor()
{
	if (maskHandle != -1) {
		DeleteMask(maskHandle);
	}
	
	if (maskScreen != -1) {
		DeleteGraph(maskScreen);
	}
}

void IrisTransitor::Update()
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
		DeleteMask(maskHandle);
		maskHandle = -1;
		break;
	default:
		break;
	}
}

void IrisTransitor::Draw()
{
	if (IsEnd()) {
		return;
	}

	float rate		= time / timeMax;
	int backRT;
	int maskedRT;
	if (irisOut) {
		backRT		= oldWindow;
		maskedRT	= newWindow;
		rate		= 1.0f - rate;
	}
	else {
		backRT		= newWindow;
		maskedRT	= oldWindow;
	}
	//
	float radius = (sizeCross )* rate;
	SetDrawScreen(maskScreen);
	ClearDrawScreen();

	DrawCircleAA(size.x / 2.0f, size.y / 2.0f, radius, 32, 0xffffff, true);

	//隠し関数(現在のグラフィックハンドルをマスクスクリーンに転送)
	SetMaskScreenGraph(maskScreen);
	//描画領域を反転する
	SetMaskReverseEffectFlag(!irisOut);

	SetDrawScreen(DX_SCREEN_BACK);
	if (irisOut) {
		SetUseMaskScreenFlag(false);
		DrawGraph(0, 0, backRT, true);
		SetUseMaskScreenFlag(true);
		DrawGraph(0, 0, maskedRT, true);
		SetUseMaskScreenFlag(false);
	}
	else {
		SetUseMaskScreenFlag(false);
		DrawGraph(0, 0, backRT, true);
		SetUseMaskScreenFlag(true);
		DrawGraph(0, 0, maskedRT, true);
		SetUseMaskScreenFlag(false);
	}
	
}

void IrisTransitor::SetMask()
{
	maskScreen	= MakeScreen(size.x, size.y, true);
	maskHandle	= CreateMaskScreen();
	sizeCross	= hypotf((float)size.x,(float)size.y )/ 2.0f;
}

void IrisTransitor::StartIris(std::string _sceneName, float _time, bool _out)
{
	Transitor::StartTransitor(_sceneName, _time);
	irisOut = _out;
	SetMask();
}
