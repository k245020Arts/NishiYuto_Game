#pragma once
#include "../../EnemyState/EnemyStateBase.h"
#include "../Boss.h"

class BossBackStep :public EnemyStateBase
{
public:
	BossBackStep();
	~BossBackStep();
	void Update()override;

	void Start()override;
	void Finish()override;
private:
	const float MaxSpeed = 50.0f;
	float speed;

	float time;
	int counter;

	VECTOR3 pPos;
};