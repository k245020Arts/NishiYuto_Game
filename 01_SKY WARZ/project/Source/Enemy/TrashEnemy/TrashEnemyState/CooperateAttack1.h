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
	//
	const Transform CollTransform = Transform(VECTOR3(0, 0, -100.0f), VZero, VECTOR3(400.0f, 0, 0));
	
	const float FlashStartTime	= 0.7f;
	const float SlowAmout		= 0.1f;
	const float SlowTime		= 0.3f;
	const float speedUpMotion	= 0.0f;
	
	const float TimeMax			= 1.0f;
	float time;
};