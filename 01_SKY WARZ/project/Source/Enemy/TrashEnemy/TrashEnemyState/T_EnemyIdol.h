#pragma once
#include "../../EnemyState/EnemyStateBase.h"

class T_EnemyIdol :public EnemyStateBase
{
public:
	T_EnemyIdol();
	~T_EnemyIdol();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;
private:
	void NormalMove();

	const float RANGESPEED = 60.0f;
	const float Gravity = -1000.0f;
	const int RandMax = 500;
	//プレイヤーの探知範囲を少しずつあげる
	float detectionRange;

	VECTOR3 setGravity;

	int counter;
};