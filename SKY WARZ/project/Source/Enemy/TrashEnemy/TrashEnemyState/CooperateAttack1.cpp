#include "CooperateAttack1.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "T_EnemyStatus.h"

CooperateAttack1::CooperateAttack1()
{
	string = Function::GetClassNameC<CooperateAttack1>();
	animId = ID::TE_C_ATTACK;
	attackParam.animID = ID::TE_C_ATTACK;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	attackParam.damagePattern = EnemyAttackBase::BACK;

	attackParam.hitDamage = 30;
	time = 0;

	attackParam.useFlash = true;
	attackParam.attackFlashStartTime = 0.7f;
	attackParam.slowAmout = 0.1f;
	attackParam.slowTime = 0.3f;
	attackParam.speedUpMotionSpeed = 0.0f;

	copyColl = VZero;
}

CooperateAttack1::~CooperateAttack1()
{
}

void CooperateAttack1::Update()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	EnemyAttackBase::Update();

	AttackInformation(enemy);

	time += Time::DeltaTimeRate();
	if (time >= 1)
		enemy->isCooperateAtk = false;
}

void CooperateAttack1::Start()
{
	const TrashEnemy* enemy = GetBase<TrashEnemy>();

	firstColl = true;

	copyColl = EnemyAttackBase::collTrans.scale;
	EnemyAttackBase::collTrans.scale = VECTOR3(400, 0, 0);

	EnemyStateBase::Start();
}

void CooperateAttack1::Finish()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	enemy->speed=enemy->eStatus->GetStatus().runSpeed;
	enemy->CooperateAtkFinish();
	enemy->DeleteCollision(&enemy->attackColl);

	EnemyAttackBase::collTrans.scale = copyColl;
}
