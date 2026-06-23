#pragma once
#include "PlayerAttackStateBase.h"

class PlayerAttack3 : public PlayerAttackStateBase
{
public:
	PlayerAttack3();
	~PlayerAttack3();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

	void StateImguiDraw()override;

private:

};
