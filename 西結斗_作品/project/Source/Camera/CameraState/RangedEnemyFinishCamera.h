#pragma once
#include "CameraStateBase.h"

class TrashEnemyGroup;
class Player;

class RangedEnemyFinishCamera :public CameraStateBase
{
public:
	RangedEnemyFinishCamera();
	~RangedEnemyFinishCamera();

	void Update()override;
	void Start()override;
	void Finish()override;

private:
	Player* player;
	TrashEnemyGroup* groupManager;
	VECTOR3 targetPos;

	const VECTOR3 PosOffset = VECTOR3(300.0f, 1000.0f, 0.0f);
};
