#pragma once
#include "PlayerAttackStateBase.h"

class Transform;

class PlayerJustAvoidAttack1 : public PlayerAttackStateBase
{
public:
	PlayerJustAvoidAttack1();
	~PlayerJustAvoidAttack1();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

	void Again();
	void StateImguiDraw() override;

private:
	float timer;
	int count;
	bool attack;
};