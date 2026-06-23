#include "MotionBlur.h"
#include "../Animator/Animator.h"
#include "../ComponentManager.h"
#include "../Transform/Transform.h"
#include "../../Common/ResourceLoader.h"

MotionBlur::MotionBlur()
{
	debugId	= 8;
	tag		= Function::GetClassNameC<MotionBlur>();
	model	= -1;
	anim	= nullptr;
}

MotionBlur::~MotionBlur()
{
}

void MotionBlur::Update()
{
	for (auto itr = blur.begin(); itr != blur.end();) {
		itr->time -= Time::DeltaTimeRate();
		//制限時間を過ぎたら消す
		if (itr->time <= 0.0f) {
			MV1DeleteModel(itr->baseModel);
			itr = blur.erase(itr);
		}
		else {
			itr++;
		}
	}
}

void MotionBlur::Draw()
{
	for (auto& m : blur) {
		MV1SetAttachAnimTime(m.baseModel, m.attackID, m.animFrame);
		//半透明で残像を作っている。
		MV1SetDifColorScale(m.baseModel, GetColorF(1.f, 1.f, 1.f, m.alpha));
		//残像をきれいにするために加算合成を行う
		SetDrawBlendMode(DX_BLENDMODE_ADD, 40);
		for (int i = 0; i < 8; i++) {
			Transform transform = m.transform;
			transform.scale += i / 50.0f * VOne;
			MV1SetMatrix(m.baseModel,transform.GetMatrix());
			MV1DrawModel(m.baseModel);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
}

void MotionBlur::Start(int _model)
{
	model	= _model;
	anim	= obj->Component()->GetComponent<Animator>();
}

void MotionBlur::MosionStart(float _time, float _alpha,ID::IDType _id, int _modelFrame)
{
	BlurInfo b;
	b.alpha					= _alpha;
	b.time					= _time;
	b.maxTime				= _time;
	b.baseModel				= MV1DuplicateModel(model);
	b.animFrame				= anim->GetCurrentFrame();
	b.transform				= *obj->GetTransform();
	b.transform.rotation.y	+= 180.0f * DegToRad;
	b.matrix				= b.transform.GetMatrix();
	b.attackID				= MV1AttachAnim(b.baseModel, 0, ResourceLoad::GetHandle(_id));

	//ブラーに使用するアニメーションの情報の記録
	MV1SetAttachAnimTime(b.baseModel, b.attackID, b.animFrame);
	MATRIX identity = MGetIdent();
	MV1SetFrameUserLocalMatrix(b.baseModel, _modelFrame, identity);

	blur.emplace_back(b);
	
}
