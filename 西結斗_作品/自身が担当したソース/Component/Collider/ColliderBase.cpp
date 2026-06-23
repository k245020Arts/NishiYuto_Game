#include "colliderBase.h"
#include "CollsionManager.h"

ColliderBase::ColliderBase()
{
	collsionEventFunction = nullptr;
	shape = CollsionInformation::SHAPE_MAX;
	collTag = CollsionInformation::TAG_MAX;
	collTransform = nullptr;
	radius = 0.0f;
	oneColl = false;
	finish = false;
}

ColliderBase::~ColliderBase()
{
	if (collTag == CollsionInformation::ENEMY) {
		int a = 0;
	}
	delete collTransform;
	CollsionManager* coll = FindGameObject<CollsionManager>();
	coll->RemoveCollList(this);
	collsionEventFunction = nullptr;
}

void ColliderBase::Update()
{
}

void ColliderBase::Draw()
{
}

void ColliderBase::CollsionAdd(const CollsionInfo& _info, const Transform& transform, std::function<void(const CollsionEventData&)> _funk)
{
	CollsionAdd(_info, transform,_funk, "");
}

void ColliderBase::CollsionAdd(const CollsionInfo& _info, const Transform& transform, std::function<void(const CollsionEventData&)> _funk, std::string _tag)
{
	shape = _info.shape;
	radius = _info.size;
	oneColl = _info.oneColl;
	collTag = _info.tag;
	finish = false;
	collsionEventFunction = _funk;

	collTransform = new Transform(transform);

	collTransform->SetParent(_info.parentTransfrom);

	CollsionManager* coll = FindGameObject<CollsionManager>();
	coll->AddPush(this);
	SetCollTag(_tag);
}