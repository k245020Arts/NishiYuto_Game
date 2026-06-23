#include "CollsionEvent.h"
#include "../ComponentManager.h"
#include "../../Player/Player.h"
#include "../../Enemy/TrashEnemy/TrashEnemy.h"
#include "../../Common/Debug/Debug.h"
#include "../../Enemy/EnemyState/EnemyStateManager.h"
#include "../../Enemy/EnemyState/EnemyStateBase.h"
#include "../../Enemy/Boss/Boss.h"
#include "../../Common/Effect/EffectBase.h"
#include "../../Camera/Camera.h"
#include "../EnemyAttackObject/BossRock/BossRockBase.h"
#include "../Physics/Physics.h"

CollsionEvent::CollsionEvent()
{
	
}

CollsionEvent::~CollsionEvent()
{
}
#define CollsionEventPtrMode

void CollsionEvent::Event(ColliderBase* _coll1, ColliderBase* _coll2, Pushback& resolver, const VECTOR3& _hitPos)
{
	using namespace CollsionInformation;

#ifdef CollsionEventPtrMode
	Tag tag1 = _coll1->GetCollTag();
	Tag tag2 = _coll2->GetCollTag();

	CollsionEventData col1EventData = SetEventData(_coll1,_coll2,resolver,_hitPos);
	CollsionEventData col2EventData = SetEventData(_coll2,_coll1,resolver,_hitPos);

	std::function<void(const CollsionEventData&)> eventFunc1 = _coll1->GetEventFunc();
	std::function<void(const CollsionEventData&)> eventFunc2 = _coll2->GetEventFunc();

	if (eventFunc1 != nullptr) {
		eventFunc1(col1EventData);
	}
	
	if (eventFunc2 != nullptr) {
		eventFunc2(col2EventData);
	}

#else
	Tag tag1 = _coll1->GetCollTag();
	Tag tag2 = _coll2->GetCollTag();

	int key = MakeKey(tag1, tag2);

	auto it = eventTable.find(key);

	if (it != eventTable.end())
	{
		(this->*(it->second))(_coll1, _coll2, resolver, _hitPos);
	}
#endif // CollsionEventPtrMode


	
}
CollsionEventData CollsionEvent::SetEventData(ColliderBase* _myCollObj, ColliderBase* _targetCollObj, Pushback& _pushInfo, const VECTOR3& _hitPos)
{
	CollsionEventData eventData;

	eventData.myTransform = _myCollObj->GetBaseObject()->GetTransform();
	eventData.myShape = _myCollObj->GetShape();
	eventData.myTag = _myCollObj->GetCollTag();
	eventData.myObject = _myCollObj->GetBaseObject();
	eventData.myColliderBase = _myCollObj;

	eventData.targetTransform = _targetCollObj->GetBaseObject()->GetTransform();
	eventData.targetShape = _targetCollObj->GetShape();
	eventData.targetTag = _targetCollObj->GetCollTag();
	eventData.targetObject = _targetCollObj->GetBaseObject();
	eventData.targetColliderBase = _targetCollObj;

	eventData.pushes = _pushInfo.GetPushInfo();
	eventData.hitPos = _hitPos;
	

	return eventData;
}
