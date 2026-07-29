#pragma once
#include "../../EnemyState/EnemyStateBase.h"

class TrashEnemy;

class Standby :public EnemyStateBase
{
public:
	Standby();
	~Standby();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;
private:
	//敵の様子見の移動
	void RotateMove(TrashEnemy* _enemy);
	//
	void CooperateMove(TrashEnemy* _enemy);
	//カメラの中にいたら攻撃できる
	void InCameraView();
	const float BACKSPEED = -20.0f;
	const float RunTimeMac = 0.5f;

	VECTOR3 vec;
	VECTOR3 pPos;
	//どこに移動するかわかる
	VECTOR3 newPos;

	float cooperateCounter;
	float range;
	float randomSpeed;
	float redefinitionCounter;
	//次のポジションを再定義する
	bool isRedefinition;

	float runTime;
};