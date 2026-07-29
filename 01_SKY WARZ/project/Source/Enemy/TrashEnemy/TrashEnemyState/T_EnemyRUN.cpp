#include "T_EnemyRUN.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Player/Player.h"

T_EnemyRun::T_EnemyRun()
{
	string = Function::GetClassNameC<T_EnemyRun>();

	targetPos = VZero;

	motionSpeed = 0;
}

T_EnemyRun::~T_EnemyRun()
{
}

void T_EnemyRun::Update()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	if (enemy->IsPlayerSpecialMove())
	{
		enemy->enemyBaseComponent.anim->SetPlaySpeed(0);
		return;
	}
	else
		enemy->enemyBaseComponent.anim->SetPlaySpeed(motionSpeed);

	if (!enemy->isCooperateAtk)
		targetPos = enemy->targetPoint;
	else if (enemy->isMovingToPlayer)
		targetPos = enemy->enemyBaseComponent.playerObj->GetTransform()->position;
	else
		targetPos = enemy->cooperateWayPoint;
	
	rotation = enemy->obj->GetTransform()->rotation;
	enemy->LookTarget(targetPos);

	enemy->NormalMove(enemy->GetStatus().runSpeed);
	
	VECTOR3 targetVec = targetPos - enemy->obj->GetTransform()->position;
	VECTOR3 playerVec = enemy->enemyBaseComponent.playerObj->GetTransform()->position - enemy->obj->GetTransform()->position;

	if (targetVec.Size() <= enemy->GetStatus().runRange || (playerVec.Size() <= enemy->GetStatus().playerRange && !enemy->isCooperateAtk))
	{
		if (!enemy->IsMovingToPlayer())
			enemy->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_STANDBY);
		else
			enemy->cAttack = true;
	}
}

void T_EnemyRun::Start()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	//‹ß‹——£‚Æ‰“‹——£‚Å‚Ìˆá‚¢‚ðÝ’è
	switch (enemy->enemyType)
	{
	case EnemyType::MELEE:
		//‹ß‹——£‚Æ‰“‹——£‚Åƒ‚[ƒVƒ‡ƒ“‚ª•Ï‚í‚é‚©‚ç‚»‚Ì‚½‚ß‚ÌÝ’è
		animId = ID::TE_RUN;

		if (!enemy->isCooperateAtk)
			targetPos = enemy->targetPoint;
		else if (enemy->isMovingToPlayer)
			targetPos = enemy->enemyBaseComponent.playerObj->GetTransform()->position;
		else
			targetPos = enemy->cooperateWayPoint;
		break;
	case EnemyType::RANGED_LEADER:
	case EnemyType::RANGED:

		enemy->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_WAITSEE);
		break;
	default:
		break;
	}

	motionSpeed = enemy->enemyBaseComponent.anim->GetPlaySpeed();

	enemy->isRunState = true;

	EnemyStateBase::Start();
}

void T_EnemyRun::Finish()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	
	enemy->isRunState = false;
}
