#pragma once
#include "CameraStateBase.h"

class PlayerHeavyAttackCamera : public CameraStateBase
{
public:
	PlayerHeavyAttackCamera();
	~PlayerHeavyAttackCamera();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	Player* player;
	float backCounter;

	const float TIMER_MAX = 1.0f;

	VECTOR3 targetPos;
	VECTOR3 currentTarget;
	float beforeTarget;
	VECTOR3 basePos;

	float zoomTimer;
	float backTimer;
};
