#include "BossDie.h"
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

BossDie::BossDie()
{
	animId		= ID::BOSS_DIE;
	//id = ID::BOSS_DIE;
	string		= Function::GetClassNameC<BossDie>();
	slowTime	= 0.0f;
}

BossDie::~BossDie()
{
}

void BossDie::Update()
{
	Boss* b = GetBase<Boss>();
	if (b->enemyBaseComponent.anim->IsFinish()) {
		//死ぬモーションが終わったらプレイヤー勝利Stateへと移行
		b->enemyBaseComponent.state->SetNoStateChange(false);
		b->enemyBaseComponent.gameManager->ChangeState(GameControler::GameState::WIN);
			
	
	}
	else {
		//死んだ瞬間はスロー再生をさせる
		if (slowTime <= 0.0f) {
			Time::ChangeDeltaRate(1.0f);
			b->enemyBaseComponent.camera->CameraShakeStop();
			b->enemyBaseComponent.shaker->ShakeFinish();
		}
		else {
			slowTime -= Time::DeltaTime();
		}
	}
}

void BossDie::Draw()
{
}

void BossDie::Start()
{
	Boss* b		= GetBase<Boss>();
	EnemyStateBase::Start();
	Time::ChangeDeltaRate(0.1f);
	slowTime	= 0.5f;
	b->enemyBaseComponent.camera->CameraShake(VOne * 10.0f, Shaker::MIX_SHAKE, false, -1.0f);
	b->enemyBaseComponent.shaker->ShakeStart(VOne * 10.0f, Shaker::MIX_SHAKE, false, -1.0f);

	//死んだときに当たり判定を消したいので当たり判定を消す。
	obj->Component()->RemoveAllComponent<SphereCollider>();
	obj->Component()->RemoveAllComponent<ModelCollider>();
	b->enemyBaseComponent.physics->SetGravity(VZero);
	SoundManager::GetInstance()->FeedInOut(Sound_ID::PLAY_BGM, 1.0f);
}

void BossDie::Finish()
{
}
