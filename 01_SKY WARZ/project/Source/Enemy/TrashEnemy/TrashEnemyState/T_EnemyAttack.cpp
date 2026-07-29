#include "T_EnemyAttack.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "../../../Player/Player.h"

T_EnemyAttack::T_EnemyAttack()
{
	string = Function::GetClassNameC<T_EnemyAttack>();
	animId = ID::TE_ATTACK;
	attackParam.animID = ID::TE_ATTACK;
	collTrans = Transform(VECTOR3(0, 0, -100), VZero, VECTOR3(480.0f, 0.0f, 0.0f));
	attackParam.damagePattern = EnemyAttackBase::NO_BACK;

	attackParam.useFlash = true;
	attackParam.attackFlashStartTime = 0.7f;
	attackParam.slowAmout = 0.1f;//3
	attackParam.slowTime = 0.1f;//3
	attackParam.speedUpMotionSpeed = 0.0f;
}

T_EnemyAttack::~T_EnemyAttack()
{
}

void T_EnemyAttack::Update()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	enemy->LookTarget(enemy->enemyBaseComponent.playerObj->GetTransform()->position);
	
	if (enemy->isCooperateAtk)
		return;

	if (!enemy->playerCloser)
		RunMove(enemy);
	else
	{
		enemy->enemyBaseComponent.anim->Play(ID::TE_ATTACK);
		EnemyAttackBase::Update();
		AttackInformation(enemy);
	}
}

void T_EnemyAttack::Draw()
{
}

void T_EnemyAttack::Start()
{
	const TrashEnemy* enemy = GetBase<TrashEnemy>();
	
	firstColl = true;
	attackParam.hitDamage = enemy->GetStatus().normalAttack1;
	counter = 0;

	EnemyStateBase::Start();
}

void T_EnemyAttack::Finish()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	enemy->isAttack = false;
	enemy->DeleteCollision(&enemy->attackColl);
}

void T_EnemyAttack::AttackInformation(TrashEnemy* _e)
{
	AttackCollsion();
	AttackSound();
	AttackFlash(ID::E_MODEL, 35, "E_AttackV");
	Trail();
	EnemyJustAvoidCollsion();

	if (_e->enemyBaseComponent.anim->IsFinish())
		_e->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_STANDBY);
}

void T_EnemyAttack::RunMove(TrashEnemy* _enemy)
{
	if (VSize(_enemy->enemyBaseComponent.playerObj->GetTransform()->position - obj->GetTransform()->position) <= _enemy->eStatus.atkRange)
	{
		_enemy->playerCloser = true;
		return;
	}
	_enemy->enemyBaseComponent.anim->Play(ID::TE_RUN);
	_enemy->NormalMove(_enemy->GetStatus().runSpeed);
}
