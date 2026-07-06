#pragma once
#include "CameraStateBase.h"

class JustAvoidAttackCamera : public CameraStateBase
{
public:
	JustAvoidAttackCamera();
	~JustAvoidAttackCamera();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;



private:
	float timer;
	float distanceTimer;
	const float MAX_TIMER = 0.8f;
	const float DISTANCE_TIMER_MAX = 3.0f;
	VECTOR3 targetPos;
	float firstRotation;
	const float CHANGE_ROTATION_Y = 20.0f;
};