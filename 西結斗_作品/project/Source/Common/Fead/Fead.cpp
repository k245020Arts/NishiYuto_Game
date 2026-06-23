#include "Fead.h"
#include "../../Screen.h"
#include "../../Component/Color/Color.h"

Fead::Fead()
{
	feedTime = 0.0f;
	feedCountMax = 0.0f;

	color = 0;;
	alpha = 0;;

	DontDestroyOnSceneChange();
	//一番最後に描画させたいので低い値を代入
	SetDrawOrder(-10000);

	feadIn = Fead::NONE;;
	easingFunc = nullptr;
}

Fead::~Fead()
{
}

void Fead::Update()
{
	
}

void Fead::Draw()
{
	if (feedTime <= 0.0f) {
		feadIn = NONE; //フェードタイムが設定させてなかったらフェードしていない状態にする
		return;
	}
	feedTime -= Time::DeltaTimeRate();
	if (feedTime <= 0.0f) {
		feedTime = 0.0f;
	}
	float rate = 0.0f;

	//フェードインとフェードアウトで処理を分ける
	if (feadIn) {
		rate = feedTime / feedCountMax;
	}
	else {
		rate = 1 - (feedTime / feedCountMax);
	}
	if (easingFunc != nullptr) {
		alpha = easingFunc(OPAQUE_COLOR, TRANSPARENT_COLOR, rate);
	}
	

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBoxAA(0, 0, Screen::WIDTH, Screen::HEIGHT, color, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Fead::FeadIn(float _time, int _color, std::function<int(int, int, float)> _func)
{
	if (IsFead()) { //フェード中なら通らない
		return;
	}
	feedCountMax = _time;
	feedTime = _time;
	alpha = 255;
	color = _color;
	easingFunc = _func;
	feadIn = FEAD_IN;
}

void Fead::FeadOut(float _time, int _color, std::function<int(int, int, float)> _func)
{
	if (IsFead()) {//フェード中なら通らない
		return;
	}
	feedCountMax = _time;
	feedTime = _time;
	alpha = 255;
	color = _color;
	easingFunc = _func;
	feadIn = FEAD_OUT;
}
