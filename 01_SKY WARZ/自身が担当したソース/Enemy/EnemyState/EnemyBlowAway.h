#pragma once
#include "EnemyStateBase.h"

class EnemyBlowAway : public EnemyStateBase
{
public:

	struct EnemyBlowAwayInfo
	{
		EnemyBlowAwayInfo() {
			EnemyBlowAwayInfo(0.0f, 0.0f, VZero, VZero, 0.0f, 0.0f);
		}
		EnemyBlowAwayInfo(float _backSpeed, float _UpSpeed, VECTOR3 _shakePower, VECTOR3 _shakeAdd,float _gravitySet,float _gravityAdd) {
			backSpeed = _backSpeed;
			UpSpeed = _UpSpeed;
			shakePower = _shakePower;
			shakeAdd = _shakeAdd;
			gravitySet = _gravitySet;
			gravityAdd = _gravityAdd;
		}

		float backSpeed;
		float UpSpeed;
		VECTOR3 shakePower;
		VECTOR3 shakeAdd;
		float gravitySet;
		float gravityAdd;
	};

	EnemyBlowAway();
	~EnemyBlowAway();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;
	//ìGÇÃêÅÇ¡îÚÇŒÇµÇÃèÓïÒÇämï€
	void EnemyBlowAwayInfoSet(EnemyBlowAwayInfo _info);

private:
	VECTOR3 pRotation;
	float GravityAdd;
	VECTOR3 shakeAdd;
	VECTOR3 shakePower;
};