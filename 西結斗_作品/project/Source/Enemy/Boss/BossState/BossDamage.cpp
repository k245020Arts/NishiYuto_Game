#include "BossDamage.h"
#include "../Boss.h"
#include "../../../State/StateManager.h"
#include "../../../Component/Animator/Animator.h"

BossDamage::BossDamage()
{
	animId		= ID::BOSS_DAMAGE;
	//id = ID::BOSS_DIE;
	string		= Function::GetClassNameC<BossDamage>();
}

BossDamage::~BossDamage()
{
}

void BossDamage::Update()
{
	Boss* boss	= GetBase<Boss>();
	//ダメージのモーションが終わったら追いかけモードへ移行
	if (boss->enemyBaseComponent.anim->IsFinish()) {
		boss->enemyBaseComponent.state->ChangeState(StateID::BOSS_RUN_S);
	}
}

void BossDamage::Draw()
{
}

void BossDamage::Start()
{
	EnemyStateBase::Start();
}

void BossDamage::Finish()
{
}
