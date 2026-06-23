#pragma once
#include "playerStateBase.h"

class PlayerDamage : public PlayerStateBase
{
public:
	PlayerDamage();
	~PlayerDamage();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

	

private:
	VECTOR3 eRotation;
};