#pragma once
#include "../../EnemyState/EnemyStateBase.h"

class BossThreat : public EnemyStateBase
{
public:
	BossThreat();
	~BossThreat();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:

};
