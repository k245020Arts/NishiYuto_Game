#pragma once
#include "../CameraState/CameraStateBase.h"

class FreeCamera : public CameraStateBase
{
public:
	FreeCamera();
	~FreeCamera();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;



private:
	float backCounter;

	const float TIMER_MAX = 0.5f;
	VECTOR3 currentTarget;
	float beforeTarget;
	void EnemyChangeDir();
	void StickMove();

	float AnglePI(float _current, float _target);
};
