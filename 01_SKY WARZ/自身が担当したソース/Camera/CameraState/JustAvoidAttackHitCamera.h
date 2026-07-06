#pragma once
#include "CameraStateBase.h"

class JustAvoidAttackHitCamera : public CameraStateBase
{
public:
	JustAvoidAttackHitCamera();
	~JustAvoidAttackHitCamera();

	void Update()override;
	void Draw() override;

	void Start()override;
	void Finish()override;


private:
	float timer;
	float distanceTimer;
	const float MAX_TIMER = 2.0f;
	VECTOR3 targetPos;
	float firstRotation;
	const float CHANGE_ROTATION_Y = 20.0f;
};