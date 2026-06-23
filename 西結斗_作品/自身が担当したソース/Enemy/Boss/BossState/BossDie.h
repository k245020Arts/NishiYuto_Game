#pragma once
#include "../../EnemyState/EnemyStateBase.h"

class BossDie : public EnemyStateBase
{
public:
	BossDie();
	~BossDie();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:

	float slowTime;

};
