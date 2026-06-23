#include "AABBCollider.h"

AABBCollider::AABBCollider()
{
	debugId = 1;
	tag = Function::GetClassNameC<AABBCollider>();
	SetTag(tag);
	aabbInfo = AABBInfo();
}

AABBCollider::~AABBCollider()
{
}

void AABBCollider::Update()
{
}

void AABBCollider::Draw()
{
}

void AABBCollider::SetUpAABBCollsion(const CollsionInfo& _info, const VECTOR3& _boxMin, const VECTOR3& _boxMax, std::function<void(const CollsionEventData&)> _funk, std::string _tag)
{
	ColliderBase::CollsionAdd(_info, Transform(), _funk,_tag);
	aabbInfo = AABBInfo(_boxMin, _boxMax);
}

void AABBCollider::SetUpAABBCollsion(const CollsionInfo& _info, const VECTOR3& _boxMin, const VECTOR3& _boxMax, std::function<void(const CollsionEventData&)> _funk)
{
	SetUpAABBCollsion(_info, _boxMin, _boxMax, _funk,"");
}
