#pragma once
#include "PlayerAttackStateBase.h"

class PlayerHeavyAttack2 : public PlayerAttackStateBase
{
public:
	PlayerHeavyAttack2();
	~PlayerHeavyAttack2();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	
};