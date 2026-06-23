#pragma once
#include "../../EnemyState/EnemyStateBase.h"
#include "../TrashEnemy.h"

class TrashEnemy;

class T_EnemyDamage :public EnemyStateBase
{
public:
	T_EnemyDamage();
	~T_EnemyDamage();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;
protected:
	float counter;
	//やられ方
	void KnockbackMove(TrashEnemy* _e, DeadData _deadData);
	//吹き飛びの軌道
	float Orbit(float a, float h, float k);
	float motionSpeed;
	//情報を一回だけ取得する
	bool isGetInformation;
	//右ベクトル
	VECTOR3 rightVec;
	//左右判定
	float side;

	float backSpeed;

	float enemyPosY;
private:
};