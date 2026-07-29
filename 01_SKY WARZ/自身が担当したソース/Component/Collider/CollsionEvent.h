#pragma once
#include "colliderBase.h"

class CollsionEvent
{
public:
	CollsionEvent();
	~CollsionEvent();

	//ÉCÉxÉìÉgî≠ê∂
	void Event(ColliderBase* _coll1,ColliderBase* _coll2, const Pushback& resolver,const VECTOR3& _hitPos);

private:

	using EventFunc = void (CollsionEvent::*)(ColliderBase*, ColliderBase*, Pushback&, const VECTOR3&);

	CollsionEventData SetEventData(ColliderBase* _myCollObj, ColliderBase* _targetCollObj, const Pushback& _pushInfo, const VECTOR3& _hitPos);

};