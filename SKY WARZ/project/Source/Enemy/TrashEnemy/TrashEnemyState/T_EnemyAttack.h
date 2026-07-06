#pragma once
#include "../../EnemyState/Attack/EnemyAttackBase.h"

class TrashEnemy;

class T_EnemyAttack :public EnemyAttackBase
{
public:
	T_EnemyAttack();
	~T_EnemyAttack();
	virtual void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

protected:
	//エフェクトなどの情報が入ってる
	void AttackInformation(TrashEnemy* _e);
private:
	float counter;

};