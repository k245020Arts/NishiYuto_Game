#include "BossFear.h"
#include "../../../Camera/Camera.h"
#include "../../../Component/Shaker/Shaker.h"
#include "../Boss.h"
#include "../../../Component/Animator/Animator.h"
#include "../../../GameControler/GameControler.h"
#include "../../../Common/Sound/SoundManager.h"
#include "../../../Component/Collider/ColliderBase.h"
#include "../../../Component/ComponentManager.h"
#include "../../../Component/Physics/Physics.h"
#include "../../../Component/Collider/ModelCollider.h"
#include "../../../Component/Collider/SphereCollider.h"
#include "../../../State/StateManager.h"

BossFear::BossFear()
{
	animId = ID::BOSS_FEAR;
	//id = ID::BOSS_DIE;
	string = Function::GetClassNameC<BossFear>();
	fearCounter = 0.0f;

}

BossFear::~BossFear()
{
}

void BossFear::Update()
{
	Boss* b = GetBase<Boss>();
	fearCounter -= Time::DeltaTimeRate();
	if (fearCounter <= 0.0f) {
		b->enemyBaseComponent.state->ChangeState(StateID::BOSS_IDOL_S);
	}
}

void BossFear::Draw()
{
}

void BossFear::Start()
{
	Boss* b = GetBase<Boss>();
	EnemyStateBase::Start();
	fearCounter = 10.0f;
}

void BossFear::Finish()
{
}
