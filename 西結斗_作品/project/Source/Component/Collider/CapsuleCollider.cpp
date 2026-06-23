#include "CapsuleCollider.h"
#include "../Transform/Transform.h"
#include "../../Common/Debug/Debug.h"

CapsuleCollider::CapsuleCollider()
{
	debugId = 35;
	tag = Function::GetClassNameC<CapsuleCollider>();
	SetTag(tag);
	capselEndTransform = nullptr;
	matrixMult = false;
}

CapsuleCollider::~CapsuleCollider()
{
	if (capselEndTransform != nullptr) {
		delete capselEndTransform;
	}
	if (matrix != nullptr) {
		matrix = nullptr;
	}
}

void CapsuleCollider::Update()
{
	if (matrixMult) {
		collTransform->position = baseStartTransform.position * *matrix;
		capselEndTransform->position = baseEndTransform.position * *matrix;
	}
}

void CapsuleCollider::Draw()
{
	/*if (matrixMult) {
		Transform startTransform = *collTransform;
		Transform EndTransform = *capselEndTransform;
		DrawCapsule3D(startTransform.position, EndTransform.position, radius, 24, 0xff0000, 0xff0000, false);
	}
	else {
		Transform startTransform = collTransform->WorldTransform();
		Transform EndTransform = capselEndTransform->WorldTransform();
		DrawCapsule3D(startTransform.position, EndTransform.position, radius, 24, 0xff0000, 0xff0000, false);
	}*/
	
}

void CapsuleCollider::Start()
{
}

void CapsuleCollider::CapsuleSet(const CollsionInfo& _info, const Transform& _endTransform, const Transform& _startTransform, float _sphere, bool _mustMatrix, MATRIX* _matrix, std::function<void(const CollsionEventData&)> _funk)
{
	CapsuleSet(_info, _endTransform, _startTransform,_sphere,_mustMatrix,_matrix, _funk, "");
}

void CapsuleCollider::CapsuleSet(const CollsionInfo& _info, const Transform& _endTransform, const Transform& _startTransform, float _sphere, bool _mustMatrix, MATRIX* _matrix, std::function<void(const CollsionEventData&)> _funk, std::string _tag)
{
	ColliderBase::CollsionAdd(_info, _startTransform, _funk, _tag);

	capselEndTransform = new Transform(_endTransform);
	capselEndTransform->SetParent(_info.parentTransfrom);
	radius = _sphere;
	matrixMult = _mustMatrix;
	matrix = _matrix;
	baseStartTransform = *GetTransform();
	baseEndTransform = *capselEndTransform;

	baseStartTransform.RemoveParent(_info.parentTransfrom);
	baseEndTransform.RemoveParent(_info.parentTransfrom);

}
