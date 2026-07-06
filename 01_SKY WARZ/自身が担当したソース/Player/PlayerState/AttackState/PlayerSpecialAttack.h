#pragma once
#include "PlayerAttackStateBase.h"

class Player;

class PlayerSpecialAttack : public PlayerAttackStateBase
{
public:
	enum PLAYER_SPECIAL_ATTACK_STATE
	{
		NO_ATTACK = -1,
		BEFORE,
		GROUND_ATTACK,
		CHARGE,
		FINAL_ATTACK,
	};

	PlayerSpecialAttack();
	~PlayerSpecialAttack();

	void Update()override;
	void Draw()override;
	void Start()override;
	void Finish()override;

	void StateImguiDraw()override;

	
	/// <summary>
	/// ïKéEãZÇÃèÛë‘ÇÃéÊìæ
	/// </summary>
	/// <returns></returns>
	PLAYER_SPECIAL_ATTACK_STATE GetSpecialAttackState() { return state; }
	/// <summary>
	/// çUåÇóÕÇÃéÊìæ
	/// </summary>
	/// <returns>çUåÇóÕ</returns>
	bool GetAttackDamage()const { return attackDamage; }

private:

	float radius;
	
	int moveNum;

	PLAYER_SPECIAL_ATTACK_STATE state;
	float chargeCounter;
	float waitCounter;
	VECTOR3 defalutRotation;
	VECTOR3 keepPos;
	VECTOR3 dir;
	float specialAngle;

	float currentAngle;
	float angleMin;
	float angleMax;
	float randAngle;

	VECTOR3 lineStart;
	VECTOR3 lineEnd;

	float moveCounter;
	float moveSpeed; 
	bool centerTo;

	int boxHandle;
	int playerHandle;

	bool zoom;
	float zoomCounter;
	float zoomCounterBase;
	float zoomRate;
	float zoomSize;

	float beforeWaitCounter;
	bool attackDamage;

	bool moveStart;

	void BeforeUpdate();
	void GroundUpdate();
	void ChargeUpdate();
	void FinalAttackUpdate();
	void SpecialRotationChange();

	void MoveStart(float _angle);

	void AddCollsion();
};
