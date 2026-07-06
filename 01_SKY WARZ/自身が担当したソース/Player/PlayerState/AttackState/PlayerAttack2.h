#pragma once
#include "PlayerAttackStateBase.h"

class PlayerAttack2 : public PlayerAttackStateBase
{
public:
	PlayerAttack2();
	~PlayerAttack2();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:

};

