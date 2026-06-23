#pragma once
#include "../../EnemyState/EnemyStateBase.h"

class BossRoar : public EnemyStateBase
{
public:
	BossRoar();
	~BossRoar();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	bool first;
};
