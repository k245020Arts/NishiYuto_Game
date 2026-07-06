#pragma once
#include "CameraStateBase.h"

class JustAvoidCamera : public CameraStateBase
{
public:
	JustAvoidCamera();
	~JustAvoidCamera();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:

    VECTOR3 defalutDistance; //通常の距離
	VECTOR3 zoomDistance; //ジャスト回避後のズーム後の距離
    VECTOR3 currentDistance; //現在の距離
    float speed;
	float changeTimer;

    float timer;
    const float MAX_TIMER = 0.05f;
	const float CHANGE_STATE_TIMER = 0.8f;
};