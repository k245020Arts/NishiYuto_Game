#pragma once
#include "EnemyStateBase.h"

class EnemyDamage : public EnemyStateBase
{
public:

	struct EnemyDamageInfo
	{
		EnemyDamageInfo() {
			EnemyDamageInfo(VZero, VZero, 0.0f,0.0f);
		}
		EnemyDamageInfo(VECTOR3 _playerFrontModeSpeed, VECTOR3 _power, float _shaketime,float _damageTime,bool _playerFrontMode = true,float _speed = 0.0f) {
			playerFrontModeSpeed = _playerFrontModeSpeed;
			shakePower = _power;
			shakeTime = _shaketime;
			damageTime = _damageTime;
			speed = _speed;
			playerFrontMode = _playerFrontMode;
		}
		bool playerFrontMode;
		VECTOR3 playerFrontModeSpeed;
		float speed;
		VECTOR3 shakePower;
		float shakeTime;
		float damageTime;
	};

	EnemyDamage();
	~EnemyDamage();

	void Update()override;
	void Draw()override;
	void Start()override;
	void Finish()override;

	void DamageInit(EnemyDamageInfo _info);

private:
	EnemyDamageInfo info;
	float timer;
	bool first;

};