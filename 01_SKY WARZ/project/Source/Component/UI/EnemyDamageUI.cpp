#include "EnemyDamageUI.h"
#include "../../Common/InputManager/PadInput.h"
#include "../../Common/InputManager/InputManager.h"
#include "../../Component/Transform/Transform.h"
#include "../../Component/Color/Color.h"
#include "../ComponentManager.h"
#include "../../Common/Easing.h"
#include "../MeshRenderer2D/MeshRenderer2D.h"

EnemyDamageUI::EnemyDamageUI()
{
	numImage = -1;
	damageNum = 0;
	movePos = VZero;
	moveMaxPos = VZero;
	timer = 0.0f;
	timerMax = 0.0f;

	scale = 0.5f;
	alpha = 255;

	feedInOutTime = 0.0f;
	feedOut = false;
	feedIn = true;

	drawNumCount = 0;
	waitTimer = 0.06f;

	worldTransform = nullptr;
	worldOffset = VZero;
	mesh2D = nullptr;

}

EnemyDamageUI::~EnemyDamageUI()
{
	//DeleteGraph(numImage);
}

void EnemyDamageUI::Update()
{
	float dt = Time::DeltaTimeRate();

	if (!feedOut)
	{
		movePos = Easing::EasingFlow<VECTOR3>(&timer,timerMax,VZero,moveMaxPos,Easing::EaseOut<VECTOR3>);

		float rate = 1.0f - (timer / timerMax);
		scale = rate * 1.0f;

		if (feedIn)
		{
			alpha = Easing::EasingFlow<int>(&feedInOutTime, feedInOutTimeMax, OPAQUE_COLOR, TRANSPARENT_COLOR, Easing::EaseOut<int>);
			if (feedInOutTime <= 0.0f)
			{
				feedInOutTime = 0.0f;
				feedIn = false;
			}
		}

		if (timer <= 0.0f)
		{
			timer = 0.0f;
			feedOut = true;
			const float FEAD_IN_OUT_TIME = 0.25f;
			feedInOutTime = FEAD_IN_OUT_TIME;
			feedInOutTime = feedInOutTimeMax;
		}
	}
	else
	{
		alpha = Easing::EasingFlow<int>(&feedInOutTime, feedInOutTimeMax, TRANSPARENT_COLOR, OPAQUE_COLOR, Easing::EaseOut<int>);
		if (feedInOutTime <= 0.0f)
		{
			feedInOutTime = 0.0f;
			obj->DestroyMe();
		}
	}
}

void EnemyDamageUI::Draw()
{
	if (numImage < 0 || worldTransform == nullptr) {
		return; //イメージがないかTransformがnullptrの時はリターン
	}

	VECTOR3 worldPos = worldTransform->position + worldOffset + movePos;

	//World座標をScreen座標に画面変換
	VECTOR3 screenPos = ConvWorldPosToScreenPos(worldPos);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	float reTime = timerMax - timer;

	//開始する位置を取得
	float startX = screenPos.x - (graphSize.x * scale * drawNumCount) * 0.5f;
	//描画する値を別の変数に保存
	int value = damageNum;
	//一文字づつ描画
	for (int i = 0; i < drawNumCount; i++)
	{
		///一桁目の文字を抽出
		int digit = value % 10;
		value /= 10;//位を一つ下げる

		//一文字一文字ずつ描画させている。
		float numTime = reTime - waitTimer * i;
		//一個一個ずつ出していて、まだ出番ではないので出さない。
		if (numTime < 0) {
			continue;
		}

		const float CHAR_TIME_MAX = 0.2f;
		float t = numTime / CHAR_TIME_MAX;

		if (t >= CHAR_TIME_MAX) {
			t = 1.0f;
		}

		const float BOUNCE = 84.0f;
		
		float bounce = Easing::SinCube(0.0f, BOUNCE,t);
		const float MAX_SCALE = 0.05f;
		float plus = Easing::SinCube(0.0f, MAX_SCALE, t);
		float numScale = scale + plus;

		DrawRectRotaGraph((int)(startX + (drawNumCount - 1 - i) * graphSize.x * scale),(int)(screenPos.y - bounce),digit * graphSize.x,
			0,graphSize.x,graphSize.y, numScale,0.0f,numImage,true);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void EnemyDamageUI::SetInformation(const VECTOR3& _offset, int _damageNum, const VECTOR3& _movePos, float _timer, int _image, Transform* _world, const VECTOR2I& _graphsize)
{
	worldTransform = _world;
	worldOffset = _offset;

	damageNum = _damageNum;
	moveMaxPos = _movePos;

	timer = _timer;
	timerMax = _timer;

	numImage = _image;
	graphSize = _graphsize;

	// 桁数計算
	int temp = damageNum;
	drawNumCount = 0;
	do {
		drawNumCount++; 
		temp /= 10;//位を一つ下げる
	} while (temp > 0);

	const float SCALE = 0.4f;
	scale = SCALE;
	alpha = 0;
	feedIn = true;
	feedOut = false;
	const float FEAD_IN_OUT_TIME = 0.2f;
	feedInOutTime = FEAD_IN_OUT_TIME;
	feedInOutTimeMax = feedInOutTime;
	obj->SetDrawOrder(-1000);
}
