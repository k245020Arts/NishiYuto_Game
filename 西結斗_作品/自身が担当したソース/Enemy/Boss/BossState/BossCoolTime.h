#pragma once
#include "../../EnemyState/EnemyStateBase.h"

class BossStatus;

class BossCoolTime:public EnemyStateBase
{
public:
	BossCoolTime();
	~BossCoolTime();

	void Update()override;

	void Start()override;
	void Finish()override;

private:
	BossStatus* bs;
	float coolTime;
};