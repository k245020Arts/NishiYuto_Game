#pragma once
#include "playerStateBase.h"

class PlayerBlowAway : public PlayerStateBase
{
public:
	PlayerBlowAway();
	~PlayerBlowAway();

	void Update()override;
	void Draw()override;

	void Start() override;
	void Finish()override;

private:
	VECTOR3 eRotation;
};
