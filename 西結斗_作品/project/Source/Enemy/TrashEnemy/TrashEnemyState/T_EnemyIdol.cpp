#include "T_EnemyIdol.h"
#include "../TrashEnemy.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "T_EnemyStatus.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Common/Random.h"

T_EnemyIdol::T_EnemyIdol()
{
	string = Function::GetClassNameC<T_EnemyIdol>();
	detectionRange = 0;

	setGravity = VZero;

	counter = 0;
}

T_EnemyIdol::~T_EnemyIdol()
{
}

void T_EnemyIdol::Update()
{
	NormalMove();
}

void T_EnemyIdol::Draw()
{
}

void T_EnemyIdol::Start()
{
	const TrashEnemy* enemy = GetBase<TrashEnemy>();

	//‹ß‹——£‚Æ‰“‹——£‚Åƒ‚[ƒVƒ‡ƒ“‚ª•Ï‚í‚é‚©‚ç‚»‚Ì‚½‚ß‚ÌÝ’è
	switch (enemy->enemyType)
	{
	case EnemyType::MELEE:
		animId = ID::TE_IDOL;
		break;
	case EnemyType::RANGED_LEADER: 
	case EnemyType::RANGED:
		animId = ID::TE_R_IDOL;
		break;
	default:
		break;
	}

	setGravity = enemy->enemyBaseComponent.physics->GetGravity();
	float addSpeed = (float)Random::GetInt(0, 500);
	enemy->enemyBaseComponent.physics->SetGravity(VECTOR3(0, -1000 - addSpeed, 0));

	EnemyStateBase::Start();
}

void T_EnemyIdol::Finish()
{
	const TrashEnemy* enemy = GetBase<TrashEnemy>();
	enemy->enemyBaseComponent.physics->SetGravity(setGravity);
}

void T_EnemyIdol::NormalMove()
{
	TrashEnemy* enemy = GetBase<TrashEnemy>();

	/*if (enemy->GetEnemyType() != enemy->EnemyType::MELEE && counter == 0)
		enemy->enemyBaseComponent.anim->Play(ID::TE_R_IDOL);
	counter = 1;*/

	if (enemy->GetPos().y >= 30)
		return;

	if (enemy->GetEnemyType() != EnemyType::MELEE)
	{
		enemy->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_WAITSEE);
		return;
	}

	VECTOR3 targetVec = enemy->obj->GetTransform()->position - enemy->enemyBaseComponent.playerObj->GetTransform()->position;
	detectionRange += Time::DeltaTimeRate() * RANGESPEED;
	if (targetVec.Size() < enemy->eStatus->GetStatus().chaseRange + detectionRange )
		enemy->enemyBaseComponent.state->ChangeState(StateID::T_ENEMY_RUN_S);
}
