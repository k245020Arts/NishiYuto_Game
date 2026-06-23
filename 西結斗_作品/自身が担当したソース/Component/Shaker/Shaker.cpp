#include "Shaker.h"
#include "../../Common/Debug/Debug.h"
#include "../../../ImGui/imgui.h"

Shaker::Shaker()
{
	pattern		= NONE;
	debugId		= 10;
	tag			= Function::GetClassNameC<Shaker>();
	transform	= nullptr;
	time		= 0.0f;
	stop		= false;
}

Shaker::~Shaker()
{
}

void Shaker::Update()
{
	if (!Debug::ShakeStop()) {
		return;
	}
	//通常タイムならタイムを減算して行う
	if (time > 0) {
		ShakePower();
		time -= Time::DeltaTimeRate();
		if (time <= 0.0f) {
			time = 0.0f;
			ShakeFinish();
		}
	}
	else if(time > -1.0f){
		ShakeFinish();
	}
	else {
		//-1.0より小さく設定すると指定がない限りShakeする
		ShakePower();
	}
}

void Shaker::Draw()
{
}

void Shaker::ShakeStart(const VECTOR3& _power, ShakePattern _pattern, bool rok, float _second)
{
	if (!Debug::ShakeStop()) { //HitStopがない状態だとスルー
		return;
	}
	if (time > 0.0f) { //現在シェイクを行っているならスルー
		return;
	}
	transform = obj->GetTransform();
	currentTransform = *transform;
	power = _power;
	pattern = _pattern;
	time = _second;
	stop = rok;
	if (stop) { //HitStopなら何も動かしたくないので時間を止める
		obj->SetObjectTimeRate(0.0f);
	}
	shakePos = VZero;
}

void Shaker::ShakeStart(const VECTOR3& _power, ShakePattern _pattern, float rok, float _second, Transform* _baseTransform)
{
	if (!Debug::ShakeStop()) {
		return;
	}
	if (time > 0.0f) {
		return;
	}
	transform = _baseTransform;
	currentTransform = *transform;
	power = _power;
	pattern = _pattern;
	time = _second;
	stop = rok;
	shakeSubPosition = _baseTransform->position;
	//ヒットストップするときにはカウントを進めないようにする。
	//アニメーションとか移動を出来なくするために必要
	if (stop) {
		obj->SetObjectTimeRate(0.0f);
	}
}

void Shaker::SetShakePower(const VECTOR3& _power)
{
	power = _power;
}

void Shaker::AddShakePower(const VECTOR3& _power)
{
	power += _power;
}

VECTOR3 Shaker::GetShake(const VECTOR3& _shake)
{
	return  _shake * (GetRand(66535) - 66535 / 2.0f) / (65535 / 2.0f);
}

void Shaker::ShakePower()
{
	//完全に止めたいときはここでポジションを止めている
	if (stop) {
		*transform = currentTransform;
	}
	shakePos = GetShake(power);
	currentShakePower = GetShakePos();
	/*switch (pattern)
	{
	case Shaker::VERTICAL_SHAKE:
		transform->position += shakePos * VECTOR3(0, 0, 1);
		break;
	case Shaker::HORIZONAL_SHAKE:
		transform->position += shakePos * VECTOR3(1, 0, 0);
		break;
	case Shaker::HEIGHT_SHAKE:
		transform->position += shakePos * VECTOR3(0, 1, 0);
		break;
	case Shaker::MIX_SHAKE:
		transform->position += shakePos * VECTOR3(1, 1, 1);
		break;
	default:
		break;
	}*/
	//transform->position += currentShakePower;
	//transform->position -= pos;
	//shakeSubPosition = transform->position - currentShakePower;
}

void Shaker::ShakeFinish()
{
	if (pattern == NONE) { //現在シェイクをしていないならスルー
		return;
	}
	pattern = NONE;
	time = 0.0f;
	currentShakePower = VZero;
	if (stop) {
		*transform = currentTransform;
		obj->SetObjectTimeRate(1.0f);
	}
	
}

VECTOR3 Shaker::GetShakePos() const
{
	//shakeのポジションをしている
	VECTOR3 shake = VZero;
	switch (pattern)
	{
	case Shaker::VERTICAL_SHAKE:
		shake = shakePos * VECTOR3(0, 0, 1);
		break;
	case Shaker::HORIZONAL_SHAKE:
		shake = shakePos * VECTOR3(1, 0, 0);
		break;
	case Shaker::HEIGHT_SHAKE:
		shake = shakePos * VECTOR3(0, 1, 0);
		break;
	case Shaker::MIX_SHAKE:
		shake = shakePos * VECTOR3(1, 1, 1);
		break;
	default:
		break;
	}
	return shake;
}

VECTOR3 Shaker::GetShakeOffset() const
{
	return currentShakePower;
}

void Shaker::ImguiDraw()
{
	ImGui::Separator();
	ImGui::InputFloat("time", &time);
	ImGui::Separator();
}
