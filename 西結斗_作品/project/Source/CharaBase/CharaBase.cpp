#include "CharaBase.h"
#include "../Component/Collider/SphereCollider.h"

CharaBase::CharaBase()
{
	lastTarget = false;;
	maxHp = 0;
	attackColl = CharaBase::CollsionSet();
	justAvoidColl = CharaBase::CollsionSet();
	hp = 0;
	specialAttackBar = 0.0f;
	specialAttackBarMax = 0.0f;
}

CharaBase::~CharaBase()
{
	/*DeleteCollision(&attackColl);
	DeleteCollision(&justAvoidColl);*/
	attackColl.instance = nullptr;
	justAvoidColl.instance = nullptr;
}

void CharaBase::Update()
{
}

void CharaBase::Draw()
{
}

void CharaBase::DeleteColliderComponent(CollsionSet* _set)
{
	//Œ`‚É‚æ‚Á‚Ä•Ï‚¦‚ç‚ê‚é‚æ‚¤‚É‚µ‚Ä‚¢‚é‚ªŒ»óSPHERE‚µ‚©Žg‚Á‚Ä‚È‚¢
	switch (_set->shape)
	{
	case CollsionInformation::SPHERE:
		obj->Component()->RemoveComponentWithTagIsCollsion<SphereCollider>(_set->collName);
		break;
	case CollsionInformation::MODEL:
		break;
	case CollsionInformation::RAY:
		break;
	case CollsionInformation::AABB:
		break;
	case CollsionInformation::SHAPE_MAX:
		break;
	default:
		break;
	}
}

void CharaBase::DeleteCollision(CollsionSet* _set)
{
	if (_set->instance != nullptr) {
		DeleteColliderComponent(_set);
		_set->instance = nullptr;
	}
}
