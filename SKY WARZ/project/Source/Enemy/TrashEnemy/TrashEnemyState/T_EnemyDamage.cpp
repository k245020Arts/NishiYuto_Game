#include "T_EnemyDamage.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "../../../Player/Player.h"
#include "../../../Common/Easing.h"
#include "../../../Component/Color/Color.h"

T_EnemyDamage::T_EnemyDamage()
{
	string = Function::GetClassNameC<T_EnemyDamage>();

	motionSpeed = 0;

	counter = 0;
	isGetInformation = false;
	rightVec = VZero;
	side = 0;

	backSpeed = 0;

	enemyPosY = 0.0f;
}

T_EnemyDamage::~T_EnemyDamage()
{
}

void T_EnemyDamage::Update()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	
	if (enemy->IsPlayerSpecialMove())//必殺技時に止まらずに動くから停止させる
	{
		enemy->enemyBaseComponent.anim->SetPlaySpeed(0);
		return;
	}
	
	enemy->enemyBaseComponent.anim->SetPlaySpeed(motionSpeed);

	KnockbackMove(enemy, enemy->deadPreset);

	if (enemy->hp > 0)
	{
		if (enemy->enemyBaseComponent.anim->IsFinish() && enemy->GetEnemyObj()->GetTransform()->position.y <= 0)
			enemy->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_STANDBY);
	}
}

void T_EnemyDamage::Draw()
{

}

void T_EnemyDamage::Start()
{
	const TrashEnemy* enemy = GetBase<TrashEnemy>();
	
	motionSpeed = enemy->enemyBaseComponent.anim->GetPlaySpeed();

	//近距離と遠距離でモーションが変わるからそのための設定
	switch (enemy->enemyType)
	{
	case EnemyType::MELEE:
		animId = ID::TE_DEAD;
		break;
	case EnemyType::RANGED_LEADER: 
	case EnemyType::RANGED :
		animId = ID::TE_R_DEAD;
		break;
	default:
		break;
	}

	isGetInformation = false;
	counter = 0;

	EnemyStateBase::Start();
}

void T_EnemyDamage::Finish()
{
	
}

void T_EnemyDamage::KnockbackMove(TrashEnemy* _e, DeadData _deadData)
{
	counter += Time::DeltaTimeRate();
	
	float offsetY = Orbit(_deadData.gravity, _deadData.timeToPeak, enemyPosY + _deadData.maxHeight);

	if (offsetY <= 0)
		offsetY = 0;
	if (!isGetInformation)
	{
		//正面べく
		VECTOR3 frontVec = VECTOR3(0, 0, 1) * MGetRotY(_e->enemyBaseComponent.playerObj->GetTransform()->rotation.y);
		//プレイヤーとのベクトル
		VECTOR3 vec = _e->enemyBaseComponent.playerObj->GetTransform()->position - _e->GetEnemyObj()->GetTransform()->position;
		// 右ベクトル
		rightVec = VECTOR3(frontVec.z, 0.0f, -frontVec.x);
		// 左右判定
		side = VDot(rightVec, vec);

		isGetInformation = true;
		backSpeed = _deadData.maxVelocity;
	}
	
	if (backSpeed >= 0)//段々減速させるため
		backSpeed -= Time::DeltaTimeRate() * 10.0f;
	else
		backSpeed = 0;
	//0以上なら右側、0以上なら左側（三項演算子）
	float dir = (side >= 0) ? 1.0f : -1.0f;
	//ポジションに適応する
	_e->GetEnemyObj()->GetTransform()->position -= rightVec * dir * backSpeed;

	_e->GetEnemyObj()->GetTransform()->position.y = offsetY;
}

float T_EnemyDamage::Orbit(float a, float h, float k)
{
	return a * (counter - h) * (counter - h) + k;
}