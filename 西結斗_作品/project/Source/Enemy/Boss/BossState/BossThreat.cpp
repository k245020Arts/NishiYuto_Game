#include "BossThreat.h"
#include "../../../State/StateManager.h"
#include "../../../Component/Animator/Animator.h"
#include "../Boss.h"

BossThreat::BossThreat()
{
	animId = ID::B_THREAT;
	string = Function::GetClassNameC<BossThreat>();
}

BossThreat::~BossThreat()
{
}

void BossThreat::Update()
{
	Boss* b = GetBase<Boss>();
	if (b->enemyBaseComponent.anim->AnimEventCan()) {
		b->enemyBaseComponent.anim->SetPlaySpeed(2.0f);
	}
	else {
		b->enemyBaseComponent.anim->SetPlaySpeed(0.8f);
	}
	if (b->enemyBaseComponent.anim->IsFinish()) {
		b->enemyBaseComponent.state->ChangeState(StateID::BOSS_RUN_S);
		b->enemyBaseComponent.anim->SetPlaySpeed(1.0f);
	}
}

void BossThreat::Draw()
{
}

void BossThreat::Start()
{
	Boss* b = GetBase<Boss>();
	EnemyStateBase::Start();
	
}

void BossThreat::Finish()
{

}
