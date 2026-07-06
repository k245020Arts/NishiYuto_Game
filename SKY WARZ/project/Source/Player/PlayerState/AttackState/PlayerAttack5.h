#pragma once
#include "PlayerAttackStateBase.h"

class Transform;

class PlayerAttack5 : public PlayerAttackStateBase
{
public:
	PlayerAttack5();
	~PlayerAttack5();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	float timer;
	void Again();

	const int ATTACK_NUMMAX = 2;
	bool avoidReady;
};