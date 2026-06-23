#include "T_EnemyWaitSee.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "../../../Common/Random.h"

T_EnemyWaitSee::T_EnemyWaitSee()
{
	animId = ID::TE_R_IDOL;

	attackCounter = 0.0f;
	targetPos = VZero;
	isLeader = false;

	moveSpeed = 0;

	pointRange = 0.0f;
}

T_EnemyWaitSee::~T_EnemyWaitSee()
{
}

void T_EnemyWaitSee::Update()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();
	enemy->LookTarget(enemy->enemyBaseComponent.playerObj->GetTransform()->position);

	Move(enemy);
}

void T_EnemyWaitSee::Start()
{
	const TrashEnemy* enemy = GetBase<TrashEnemy>();
	
	targetPos = enemy->TargetPoint();

	switch (enemy->enemyType)
	{
	case EnemyType::RANGED:
		moveSpeed = NormalMoveSpeed;
		pointRange = 400.0f + 200.0f * (float)Random::GetReal();
		break;
	case EnemyType::RANGED_LEADER:
		pointRange = 400;
		moveSpeed = LeaderMoveSpeed;
		isLeader = true;
		break;
	default:
		break;
	}

	EnemyStateBase::Start();
}

void T_EnemyWaitSee::Finish()
{
}

void T_EnemyWaitSee::Move(TrashEnemy* _enemy)
{
	if (_enemy->GetEnemyType() == EnemyType::RANGED_LEADER)
		if (_enemy->GetPos().y > 0.0f)
			return;

	targetPos = _enemy->TargetPoint();
	const VECTOR3 enePos = _enemy->GetPos();

	if (VSize(targetPos - enePos) < pointRange)
		return;

	VECTOR3 dir = VNorm(targetPos - enePos);
	_enemy->GetEnemyObj()->GetTransform()->position+= dir * moveSpeed;
}
