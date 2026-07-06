#include "rayCollider.h"
#include "../Transform/Transform.h"
#include "../../Common/Debug/Debug.h"

RayCollider::RayCollider()
{
	debugId = 20;
	tag = Function::GetClassNameC<RayCollider>();
	SetTag(tag);
	endTransform = nullptr;
}

RayCollider::~RayCollider()
{
	if (endTransform != nullptr) {
		delete endTransform;
		endTransform = nullptr;
	}
	
}

void RayCollider::Update()
{
}

void RayCollider::Draw()
{
	if (Debug::RayColliderDraw()) { //デバック表示モードがオフなら何もしない
		DrawLine3D(collTransform->WorldTransform().position, endTransform->WorldTransform().position, 0xff0000);
	}
}

void RayCollider::Start()
{
	
}

void RayCollider::RaySet(const CollsionInfo& _info, const Transform& _transform, const Transform& _endTransform, std::function<void(const CollsionEventData&)> _funk)
{
	RaySet(_info, _transform,_endTransform, _funk, "");
}

void RayCollider::RaySet(const CollsionInfo& _info, const Transform& _transform, const Transform& _endTransform, std::function<void(const CollsionEventData&)> _funk, std::string _tag)
{
	ColliderBase::CollsionAdd(_info, _transform, _funk,_tag);

	endTransform = new Transform(_endTransform);
	endTransform->SetParent(_info.parentTransfrom);
}
