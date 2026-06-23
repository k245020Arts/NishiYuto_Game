#include "ModelCollider.h"
#include "../../Common/Debug/Debug.h"

ModelCollider::ModelCollider()
{
	debugId = 2;
	tag = Function::GetClassNameC<ModelCollider>();
	SetTag(tag);
	modelHandle = -1;
}

ModelCollider::~ModelCollider()
{
	MV1TerminateCollInfo(modelHandle, -1);
	if (modelHandle >= 0) {
		MV1DeleteModel(modelHandle);
		modelHandle = -1;
	}
	
}

void ModelCollider::Update()
{
	Transform trans = collTransform->WorldTransform();
	MV1SetMatrix(modelHandle, collTransform->WorldTransform().GetMatrix());

	int a = MV1RefreshCollInfo(modelHandle, -1);
}

void ModelCollider::Draw()
{
	if (!Debug::ModelColliderDraw()) { //デバック表示モードがオフなら何もしない
		return;
	}
	MV1DrawModel(modelHandle);
}

void ModelCollider::ModelColliderSet(const CollsionInfo& _info, const Transform& _transform, int _modelHandle, std::function<void(const CollsionEventData&)> _funk)
{
	ModelColliderSet(_info, _transform, _modelHandle, _funk,"");
}

void ModelCollider::ModelColliderSet(const CollsionInfo& _info, const Transform& _transform, int _modelHandle, std::function<void(const CollsionEventData&)> _funk, std::string _tag)
{
	ColliderBase::CollsionAdd(_info, _transform, _funk,_tag);

	modelHandle = _modelHandle;
	int i = MV1SetupCollInfo(modelHandle, -1, 8, 8, 8);
}
