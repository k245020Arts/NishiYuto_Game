#pragma once
#include "../../EnemyState/EnemyStateBase.h"

class BossWin :public EnemyStateBase
{
public:
	BossWin();
	~BossWin();

	void Update()override;

	void Start()override;
	void Finish()override;
private:

};