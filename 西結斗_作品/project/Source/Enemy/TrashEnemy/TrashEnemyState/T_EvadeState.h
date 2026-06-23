#pragma once
#include "../../EnemyState/EnemyStateBase.h"

class T_Evade :public EnemyStateBase
{
public:
	T_Evade();
	~T_Evade();

	void Update()override;
	void Start()override;
	void Finish()override;
private:
	const float BackSpeed = 10.0f;
	float targetRotY;
	VECTOR3 targetPos;

	VECTOR3 copyePos;

	bool isIdolAmim;
};