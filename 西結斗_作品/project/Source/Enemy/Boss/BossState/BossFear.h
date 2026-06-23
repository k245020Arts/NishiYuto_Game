#pragma once
#include "../../EnemyState/EnemyStateBase.h"

class BossFear : public EnemyStateBase
{
public:
	BossFear();
	~BossFear();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:
	float fearCounter;
};