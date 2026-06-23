#pragma once
#include "../Component.h"

class EnemyAttackObject : public Component
{
public:
	EnemyAttackObject();
	virtual ~EnemyAttackObject();

	virtual void Update()override;
	virtual void Draw()override;

private:

};
