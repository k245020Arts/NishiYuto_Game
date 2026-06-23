#pragma once
#include "PlayerAttackStateBase.h"

class PlayerAttack4 : public PlayerAttackStateBase
{
public:
	PlayerAttack4();
	~PlayerAttack4();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	float timer;
	bool avoidReady;
};
