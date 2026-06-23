#pragma once
#include "PlayerAttackStateBase.h"
#include "../../../Component/Transform/Transform.h"

class PlayerAttack1 : public PlayerAttackStateBase
{
public:
	PlayerAttack1();
	~PlayerAttack1();
	void Update()override;
	void Draw()override;

	void Start() override;
	void Finish()override;

private:

	Transform targetTrans;
	VECTOR3 dist;
	VECTOR3 norm;
	
	float time;

	const float ATTACK_MOVE_DIST = 3000.0f;
};
