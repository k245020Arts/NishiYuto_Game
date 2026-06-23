#include "ResultUi.h"
#include "../Component/Transform/Transform.h"
#include "../Common/Easing.h"
#include "../Screen.h"
#include "../Component/Color/Color.h"

ResultUi::ResultUi()
{
	hImage = -1;
	win = false;
	normalExrate = 0.0f;
	addExrate = 0.0f;
	start = false;
	addCount = 0.0f;
	addDrawValue = 0;
	scaleCount = 0.0f;

	debugId = 35;
	tag = Function::GetClassNameC<ResultUi>();
	ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "Win", ID::WIN);
	ResourceLoad::LoadImageGraph(ResourceLoad::IMAGE_PATH + "Lose", ID::LOSE);

	flashTime = 0.0f;
}

ResultUi::~ResultUi()
{
}

void ResultUi::Update()
{
	if (!start) { //UI表示が始まってなかったらスルー
		return;
	}
	addCount = max(addCount - Time::DeltaTimeRate() * 1.0f, 0.0f);
	scaleCount = max(scaleCount - Time::DeltaTimeRate() * 1.5f, 0.0f);
	normalExrate = Easing::EaseInBack<float>(1.0f, 0.0f, scaleCount);
	addDrawValue = Easing::EaseIn<int>(0, 30, addCount);
	addExrate += 1.0f;
}

void ResultUi::Draw()
{
	Transform* transform = obj->GetTransform();
	if (win) {
		//勝ちの時の表示
		const int LOOP_NUM = 10;
		double size = (double)addExrate / LOOP_NUM;
		SetDrawBlendMode(DX_BLENDMODE_ADD, addDrawValue);
		//加算合成
		for (int i = 0; i < LOOP_NUM; i++) {
			double t = (double)i / LOOP_NUM;
			double s = size * (1.0 + t * 0.3);
			DrawRotaGraph((int)transform->position.x, (int)transform->position.y, s, 0.0f, hImage, true);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		double scale = (double)normalExrate;
		if (scale > 1.0) {
			scale = 1.0;
		}
		DrawRotaGraph((int)transform->position.x, (int)transform->position.y, scale, 0.0, hImage, true);


		if (addCount > flashTime)
		{
			int flashAlpha = (int)((1.0f - addCount / flashTime) * OPAQUE_COLOR);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, flashAlpha);
			DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, 0xffffff, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
	else {
		//負けの時の表示

		float t = min(1.0f - addCount, 1.0f);

		// 画面暗転
		int fade = (int)(t * 180);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade);
		const int BLACK = GetColor(0, 0, 0);
		DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, BLACK, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// 少し上から落とす
		float offsetY = (1.0f - t) * -50.0f;

		// 少し暗めに描画
		SetDrawBright(180, 180, 180);
		DrawRotaGraph((int)transform->position.x, (int)transform->position.y + (int)offsetY, normalExrate, 0.0, hImage, true);
		SetDrawBright(255, 255, 255);
	}
}

void ResultUi::ResultStart(bool _win)
{
	if (start) { //すでに始まってたら
		return;
	}
	if (_win) {
		//勝ちの時は勝ちの画像を表示
		hImage = ResourceLoad::GetHandle(ID::WIN);
	}
	else {
		//負けの時は負けの画像を表示
		hImage = ResourceLoad::GetHandle(ID::LOSE);
	}
	
	win = _win;
	start = true;
	addCount = 1.0f;
	addDrawValue = 0;
	scaleCount = 1.0f;
	flashTime = 0.8f;
}