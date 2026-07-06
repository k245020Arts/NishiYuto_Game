#pragma once
#include "PlayerAttackStateBase.h"

class PlayerHeavyAttack : public PlayerAttackStateBase
{
public:
	PlayerHeavyAttack();
	~PlayerHeavyAttack();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	float chargeCount;
};