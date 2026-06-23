#include "CutSceneBox.h"
#include "../Screen.h"
#include "../Component/Color/Color.h"

CutSceneBox::CutSceneBox()
{
	SetDrawOrder(-100000);

	feedTime = -1.0f;
	feedCountMax = -1.0f;

	alpha = 0;
	easingFunc = nullptr;

	feadIn = false;
	boxPosY = 0;
}

CutSceneBox::~CutSceneBox()
{
}

void CutSceneBox::Update()
{
}

void CutSceneBox::Draw()
{
	if (boxPosY < 0) {
		return;
	}
	feedTime -= Time::DeltaTimeRate();
	if (feedTime <= 0.0f) {
		feedTime = 0.0f;
	}
	float rate = 0.0f;

	if (feadIn) {
		rate = feedTime / feedCountMax;
	}
	else {
		rate = 1 - (feedTime / feedCountMax);
	}
	if (easingFunc != nullptr) {
		boxPosY = easingFunc(BOX_SIZE, 0, rate);
	}
	

	/*SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);*/
	DrawBoxAA(0.0f, 0.0f, (float)Screen::WIDTH, (float)boxPosY, 0x000000, true);
	DrawBoxAA(0.0f, (float)(Screen::HEIGHT - boxPosY), (float)Screen::WIDTH, (float)Screen::HEIGHT, 0x000000, true);
	/*SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);*/
}

void CutSceneBox::StartBox(float _time, int _color, std::function<int(int, int, float)> _func)
{
	if (IsFead()) { //フェード中なら動かさない
		return;
	}
	feedCountMax = _time;
	feedTime = _time;
	alpha = OPAQUE_COLOR;
	//color = _color;
	easingFunc = _func;
	feadIn = true;
	boxPosY = 1;
}

void CutSceneBox::FinishBox(float _time, int _color, std::function<int(int, int, float)> _func)
{
	if (IsFead()) { //フェード中なら動かさない
		return;
	}
	feedCountMax = _time;
	feedTime = _time;
	alpha = OPAQUE_COLOR;
	//color = _color;
	easingFunc = _func;
	feadIn = false;
}
