#pragma once
#include "../../EnemyState/EnemyStateBase.h"

class TrashEnemy;

class T_EnemyWaitSee : public EnemyStateBase
{
public:
	T_EnemyWaitSee();
	~T_EnemyWaitSee();

	void Update()override;

	void Start()override;
	void Finish()override;

	bool IsLeader() { return isLeader; }
private:
	void Move(TrashEnemy* _enemy);

	VECTOR3 targetPos;
	bool isLeader;
	float attackCounter;

	float moveSpeed;
	const float LeaderMoveSpeed = 10.0f;
	const float NormalMoveSpeed = 6.0f;

	float pointRange;
};