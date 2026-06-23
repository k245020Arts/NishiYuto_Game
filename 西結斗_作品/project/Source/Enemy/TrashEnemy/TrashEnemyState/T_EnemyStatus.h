#pragma once

class T_EnemyStatus
{
private:
	struct Status
	{
		const float normalAttack1 = 20.0f;	//攻撃１のダメージ数値
		
		const float maxHp = 1500.0f;			//最大HP
		const float defense = 500.0f;
		const float coolTime = 30.0f;			//一段目の攻撃までの時間
		const float runSpeed = 40.0f;			//移動速度
		const float range = 2000.0f;			//idelとrunの切り替わり
		const float atkRang = 600.0f;			//runと攻撃の切り替わり
		const float playerRang = 1200.0f;
		const float chaseRange = 1500.0f;		//プレイヤーと離れたときに切り替わる
		const float cooperateRange = 200.0f;
	};
	Status status;

public:
	T_EnemyStatus() { ; }
	Status GetStatus() { return status; }
};