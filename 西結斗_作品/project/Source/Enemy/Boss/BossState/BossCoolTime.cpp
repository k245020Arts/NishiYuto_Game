#include "BossCoolTime.h"
#include "../Boss.h"
#include "../../../State/StateManager.h"
#include "../../../Component/Animator/Animator.h"
#include "BossStatus.h"

BossCoolTime::BossCoolTime()
{
	animId		= ID::B_COOLTIME;
	string		= Function::GetClassNameC<BossCoolTime>();
	//id = ID::B_COOLTIME;
	bs			= nullptr;
	coolTime	= 0.0f;
}

BossCoolTime::~BossCoolTime()
{
	
}

void BossCoolTime::Update()
{
	Boss* b = GetBase<Boss>();
	b->LookPlayer();
	bs			= b->bs;

	//UŒ‚‚Ü‚Å‚ÌŽžŠÔiŒã‚ÅC³j
	coolTime	+= Time::DeltaTimeRate();
	if (coolTime >= b->coolTime)
		b->enemyBaseComponent.state->ChangeState(StateID::ATTACK_SORTING_S);
}

void BossCoolTime::Start()
{
	Boss* b = GetBase<Boss>();
	coolTime = 0;
	b->GetAttackCoolTime();
	EnemyStateBase::Start();
}

void BossCoolTime::Finish()
{
}
