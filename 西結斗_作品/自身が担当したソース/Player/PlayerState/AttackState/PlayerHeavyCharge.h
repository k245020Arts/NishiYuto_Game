#pragma once
#include "PlayerAttackStateBase.h"

class PlayerHeavyCharge : public PlayerAttackStateBase
{
public:
	PlayerHeavyCharge();
	~PlayerHeavyCharge();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	float chargeCount;
	bool chargeFinish;
	int baseFrequ;
	bool maxCharge;
};
