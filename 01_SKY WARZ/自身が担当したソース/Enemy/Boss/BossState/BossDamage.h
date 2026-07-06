#pragma once
#include "../../EnemyState/EnemyStateBase.h"

class BossDamage : public EnemyStateBase
{
public:
	BossDamage();
	~BossDamage();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;


private:

};
