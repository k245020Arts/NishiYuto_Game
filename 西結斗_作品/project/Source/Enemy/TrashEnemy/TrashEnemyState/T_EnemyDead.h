#pragma once
#include "T_EnemyDamage.h"

class T_EnemyDead :public T_EnemyDamage
{
public:
	T_EnemyDead();
	~T_EnemyDead();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;
private:
	const float SPEED = 20.0f;
	const float FADE_SPEED = 1.0f;
	
	float fadeCounter;
};