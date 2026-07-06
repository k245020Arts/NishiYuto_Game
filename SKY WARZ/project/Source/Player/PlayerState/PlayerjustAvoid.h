#pragma once
#include "playerStateBase.h"
#include "../../Component/Transform/Transform.h"

class BlurScreen;

class PlayerJustAvoid : public PlayerStateBase
{
public:
	PlayerJustAvoid();
	~PlayerJustAvoid();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

	

private:
	float easingCount;
	float cameraAngle;
	VECTOR3 cameraRotation;
	float cameraLeap;
	bool attack;
	int num;

	BlurScreen* blurScreen;
	float startTimer;
	float volume;
	void JustAvoidRotation();
};
