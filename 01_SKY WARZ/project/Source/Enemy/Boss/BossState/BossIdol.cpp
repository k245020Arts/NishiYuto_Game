#include "BossIdol.h"
#include "../Boss.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../State/StateManager.h"
#include "BossStatus.h"

BossIdol::BossIdol()
{
	animId = ID::B_IDOL;
	//id = ID::B_IDOL;
	string = Function::GetClassNameC<BossIdol>();
}

BossIdol::~BossIdol()
{
}

void BossIdol::Update()
{
	Boss* b = GetBase<Boss>();

	VECTOR3 targetVec = b->bossTransform->position - b->enemyBaseComponent.playerObj->GetTransform()->position;
	//ˆê’è‹——£‚Ü‚Å‹ß‚Ã‚¢‚½‚çƒAƒEƒg
	if (targetVec.Size() < b->bs->GetStatus().chaseRange)
		b->enemyBaseComponent.state->ChangeState(StateID::BOSS_RUN_S);
	timer -= Time::DeltaTimeRate();
	//ˆê’èˆÈã~‚Ü‚Á‚½‚çUŒ‚‚É“ü‚é
	if (timer <= 0.0f) {
		b->enemyBaseComponent.state->ChangeState(StateID::ATTACK_SORTING_S);
	}
}

void BossIdol::Draw()
{
}

void BossIdol::Start()
{
	EnemyStateBase::Start();
	timer = 1.0f;
	//Boss* b = GetBase<Boss>();
	//b->enemyBaseComponent.anim->Play(animId);
}

void BossIdol::Finish()
{
}
