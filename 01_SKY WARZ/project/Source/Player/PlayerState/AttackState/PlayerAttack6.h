#pragma once
#include "PlayerAttackStateBase.h"

class Transform;

class PlayerAttack6 : public PlayerAttackStateBase
{
public:
	PlayerAttack6();
	~PlayerAttack6();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:

};