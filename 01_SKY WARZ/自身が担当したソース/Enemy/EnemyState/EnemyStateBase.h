#pragma once
#include "../../State/StateBase.h"
#include "../../Common/ID/ID.h"
#include "../../Component/Transform/Transform.h"

class EnemyStateBase : public StateBase
{
public:
	EnemyStateBase();
	~EnemyStateBase();

	void Update()override;

	void Start()override;


	ID::IDType GetAnimId() { return animId; }

	

protected:
	ID::IDType animId;
	bool firstColl;
	Transform collTrans;
	float attackTime;
	bool sound;

	float fallFrame;
	float animSlowCounter;
	float keepAnimSpeed;
	bool slowMode;
};