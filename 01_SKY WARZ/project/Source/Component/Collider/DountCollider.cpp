#include "DountCollider.h"

DountCollider::DountCollider()
{
	debugId = 23;
	tag = Function::GetClassNameC<DountCollider>();
}

DountCollider::~DountCollider()
{
}

void DountCollider::Update()
{
	/*collTransform->scale.x += speed;
	outRadius += speed;*/
}

void DountCollider::Draw()
{
	/*DrawSphere3D(collTransform->WorldTransform().position, collTransform->scale.x, 10, 0xff0000, 0xff0000, false);
	DrawSphere3D(collTransform->WorldTransform().position, outRadius, 10, 0xffff00, 0xffff00, false);*/
}

void DountCollider::DountSet(const CollsionInfo& _info, const Transform& _transform, float _outRadius, std::function<void(const CollsionEventData&)> _funk)
{
	ColliderBase::CollsionAdd(_info, _transform, _funk);

	outRadius = _outRadius;
}
