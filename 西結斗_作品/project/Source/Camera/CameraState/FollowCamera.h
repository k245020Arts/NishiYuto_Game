#pragma once
#include "CameraStateBase.h"

class FollowCamera : public CameraStateBase
{
public:
	FollowCamera();
	~FollowCamera();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

	

private:
	VECTOR3 SetTarget();
	Player* player;
	bool enemyShake;
	bool playerShake;
	float backCounter;
	bool changeState;
	const float TIMER_MAX = 0.5f;

	VECTOR3 targetPos;
	VECTOR3 currentTarget;
	VECTOR3 addPos;
};