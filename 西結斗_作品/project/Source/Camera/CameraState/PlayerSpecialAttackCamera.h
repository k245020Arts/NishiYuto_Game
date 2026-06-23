#pragma once
#include "CameraStateBase.h"

class Player;

class PlayerSpecialAttackCamera : public CameraStateBase
{
public:
	PlayerSpecialAttackCamera();
	~PlayerSpecialAttackCamera();

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
};
