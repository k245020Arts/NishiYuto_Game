#include "EffectBase.h"
#include "../../Component/Object/Object3D.h"
#include "../ResourceLoader.h"
#include "../Easing.h"
#include "EffectManager.h"

EffectBase::EffectBase()
{
	speed	= 1.0f;
	debugId = 22;
	tag		= Function::GetClassNameC<EffectBase>();
	effectColor = nullptr;
	effectPlay = nullptr;
	effectStop = nullptr;
	feedInTime = 0.0f;
	hPlayHandle = -1;
	id = Effect_ID::EFFECT_MAX;
	parent = nullptr;
	time = 0.0f;
	transform = nullptr;
}

EffectBase::~EffectBase()
{
	delete transform;
	(this->*effectStop)();
	EffectManager::GetInstance()->RemoveEffekseer(obj);
}

void EffectBase::Update() 
{
	(this->*effectPlay)();
	if (feedInTime <= 0.0f) {
		time		-= Time::DeltaTimeRate();
		//再生が終了をしたら破棄
		if (time <= 0.0f) {
			(this->*effectStop)();
			obj->DestroyMe();
			active	= false;
		}
	}
	else {
		feedInTime		-= Time::DeltaTimeRate();
		if (feedInTime <= 0.0f) {
			feedInTime	= 0.0f; //0に丸めておかないと、Easingの処理がおかしくなるのでまとめる
		}
		float rate		= 1 - (feedInTime / FEEDIN_TIME);
		float amout		= Easing::EaseIn(float(TRANSPARENT_COLOR), float(OPAQUE_COLOR), rate);
		
		Color::Rgb rgb	= WHITE;
		//白に半透明情報を付与
		rgb.a = amout;
		(this->*effectColor)(rgb);
	}
	
}

void EffectBase::Draw() {

}

void EffectBase::EffectInit(const Transform& _transform, BaseObject* _parent, Effect_ID::EFFECT_ID _id, float _time, bool effect3D)
{
	//transform = new Transform(_transform);
	BaseObject* base = GetBaseObject();
	base->GetTransform()->position = _transform.position;
	base->GetTransform()->rotation = _transform.rotation;
	base->GetTransform()->scale = _transform.scale;
	//親のトランスフォームがついていたら、親子関係を付ける
	if (_parent != nullptr) {
		parent = _parent;
		parent->AddChild(GetBaseObject(), true);
		//transform->SetParent(_parent->GetTransform());
	}

	id = _id;
	time = _time;
	
	//3Dなら3Dで使う関数。2Dなら2Dで使う関数をセット
	if (effect3D) {
		effectPlay	= &EffectBase::EffectPlay3D;
		effectStop	= &EffectBase::EffectStop3D;
		effectColor = &EffectBase::SetColor3D;
		hPlayHandle = PlayEffekseer3DEffect(ResourceLoad::GetEffectHandle(id));
	}
	else {
		effectPlay	= &EffectBase::EffectPlay2D;
		effectStop	= &EffectBase::EffectStop2D;
		effectColor = &EffectBase::SetColor2D;
		hPlayHandle = PlayEffekseer2DEffect(ResourceLoad::GetEffectHandle(id));
	}
	feedInTime		= FEEDIN_TIME;;
}

void EffectBase::EffectPlay2D()
{
	Transform worldTrans = GetBaseObject()->GetTransform()->WorldTransform();
	if (parent != nullptr) {
		worldTrans = parent->GetTransform()->WorldTransform();
	}
	

	SetPosPlayingEffekseer2DEffect(hPlayHandle, worldTrans.position.x, worldTrans.position.y, worldTrans.position.z);
	SetRotationPlayingEffekseer2DEffect(hPlayHandle, worldTrans.rotation.x, worldTrans.rotation.y, worldTrans.rotation.z);
	SetScalePlayingEffekseer2DEffect(hPlayHandle, worldTrans.scale.x, worldTrans.scale.y, worldTrans.scale.z);

	SetSpeedPlayingEffekseer2DEffect(hPlayHandle, speed);
}

void EffectBase::EffectPlay3D()
{
	Transform worldTrans = GetBaseObject()->GetTransform()->WorldTransform();

	SetPosPlayingEffekseer3DEffect(hPlayHandle, worldTrans.position.x, worldTrans.position.y, worldTrans.position.z);
	SetRotationPlayingEffekseer3DEffect(hPlayHandle, worldTrans.rotation.x, worldTrans.rotation.y, worldTrans.rotation.z);
	SetScalePlayingEffekseer3DEffect(hPlayHandle, worldTrans.scale.x, worldTrans.scale.y, worldTrans.scale.z);

	SetSpeedPlayingEffekseer3DEffect(hPlayHandle, speed);
}

void EffectBase::EffectStop2D()
{
	StopEffekseer2DEffect(hPlayHandle);
}

void EffectBase::EffectStop3D()
{
	StopEffekseer3DEffect(hPlayHandle);
}

void EffectBase::EffectStop()
{
	(this->*effectStop)();
	active = false;
}

void EffectBase::EffectSpeedSet(float _speed)
{
	speed = _speed;
}

void EffectBase::SetColor2D(Color::Rgb _rgb)
{
	SetColorPlayingEffekseer2DEffect(hPlayHandle,(int) _rgb.r, (int)_rgb.g, (int)_rgb.b, (int)_rgb.a);
}

void EffectBase::SetColor3D(Color::Rgb _rgb)
{
	SetColorPlayingEffekseer3DEffect(hPlayHandle, (int)_rgb.r, (int)_rgb.g, (int)_rgb.b, (int)_rgb.a);
}

void EffectBase::SetColor(Color::Rgb _rgb)
{
	(this->*effectColor)(_rgb);
}

void EffectBase::ParentTransformRemove()
{
	parent->GetTransform()->RemoveChild(obj->GetTransform());
}
