#pragma once

class BossStatus
{
private:
	struct Status
	{
		float normalAttack1 = 50.0f;	//攻撃１のダメージ数値
		float normalAttack2 = 50.0f;	//攻撃２のダメージ数値
		float normalAttack3 = 80.0f;	//攻撃３のダメージ数値

		int maxAttack = 4;		//最大連続攻撃の数
		float maxHp = 30000.0f;		//最大HP
		float coolTime = 30.0f;	//一段目の攻撃までの時間
		float runSpeed = 8.0f;		//ボスの移動速度
		float fallFrame = 50.0f;	//次の攻撃の間隔を短くする
		float motionSpeed = 1.4f;		//攻撃モーションの最大速度
		float range = 1400.0f;	//敵が攻撃するまでのプレイヤーとの距離
		float chaseRange = 5000.0f;	//プレイヤーと離れたときに切り替わる
	};
	Status status;

public:
	BossStatus() { ; }
	Status GetStatus() { return status; }
};