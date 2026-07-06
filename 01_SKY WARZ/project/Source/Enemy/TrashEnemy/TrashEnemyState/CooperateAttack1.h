#pragma once
#include "T_EnemyAttack.h"

class CooperateAttack1 :public T_EnemyAttack
{
public:
	CooperateAttack1();
	~CooperateAttack1();
	void Update()override;

	void Start()override;
	void Finish()override;
private:
	VECTOR3 rotation;
	VECTOR3 velocity;

	VECTOR3 copyColl;

	float time;
};