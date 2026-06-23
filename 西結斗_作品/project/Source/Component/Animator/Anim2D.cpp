#include "Anim2D.h"

Anim2D::Anim2D()
{
	debugId = 20;
	tag		= Function::GetClassNameC<Anim2D>();

	playing = false;;
	animCounter = 0.0f;
	graphNum = 0;
	animSpeed = 0.0f; 
}

Anim2D::~Anim2D()
{
}

void Anim2D::Update()
{
	if (!playing) {
		return; //アニメーションが動いてないならリターン
	}
	animCounter += Time::DeltaTime() * animSpeed;

	//オーバーフロー対策
	const float LIMIT_ANIM_COUNTER = 100000000.0f;
	if (animCounter >= LIMIT_ANIM_COUNTER) {
		animCounter = 0.0f;
	}
}

void Anim2D::AnimSetting(float _speed, int num)
{
	animSpeed	= _speed;
	graphNum	= num;
	playing		= true;
}
