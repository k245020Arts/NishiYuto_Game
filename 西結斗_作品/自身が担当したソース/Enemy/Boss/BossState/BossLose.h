#pragma once
#include "../../EnemyState/EnemyStateBase.h"

class BossLose : public EnemyStateBase
{
public:
	BossLose();
	~BossLose();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:

};

