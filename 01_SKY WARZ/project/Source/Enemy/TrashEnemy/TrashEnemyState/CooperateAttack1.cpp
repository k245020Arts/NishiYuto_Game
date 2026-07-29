#include "CooperateAttack1.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"

CooperateAttack1::CooperateAttack1()
{
	string = Function::GetClassNameC<CooperateAttack1>();
	animId = ID::TE_C_ATTACK;
	attackParam.animID = ID::TE_C_ATTACK;
	collTrans = CollTransform;
	attackParam.damagePattern = EnemyAttackBase::BACK;

	attackParam.useFlash = true;
	attackParam.attackFlashStartTime = FlashStartTime;
	attackParam.slowAmout = SlowAmout;
	attackParam.slowTime = SlowTime;
	attackParam.speedUpMotionSpeed = speedUpMotion;

	time = 0.0f;
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

	//途中でfalseにしてダメージ耐性をオフにするため
	time += Time::DeltaTimeRate();
	if (time >= TimeMax)
		enemy->isCooperateAtk = false;
}

void CooperateAttack1::Start()
{
	const TrashEnemy* enemy = GetBase<TrashEnemy>();

	firstColl = true;
	attackParam.hitDamage = enemy->GetStatus().C_Attack1Damage;

	copyColl = EnemyAttackBase::collTrans.scale;
	//EnemyAttackBase::collTrans.scale = VECTOR3(400, 0, 0);

	EnemyStateBase::Start();
}

void CooperateAttack1::Finish()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	enemy->CooperateAtkFinish();
	enemy->DeleteCollision(&enemy->attackColl);

	EnemyAttackBase::collTrans.scale = copyColl;
}
