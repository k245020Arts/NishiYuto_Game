#include "PlayerDie.h"
#include "../../Component/Animator/Animator.h"
#include "../../GameControler/GameControler.h"
#include "../../Camera/Camera.h"
#include "../player.h"
#include "../../Common/Sound/SoundManager.h"
#include "../../Component/Collider/SphereCollider.h"
#include "../../Enemy/EnemyManager.h"
#include "../../State/StateManager.h"

PlayerDie::PlayerDie()
{
	animId	= ID::P_DIE;
	//id = ID::P_DIE;
	string	= Function::GetClassNameC<PlayerDie>();
	//one		= false;
}

PlayerDie::~PlayerDie()
{
}

void PlayerDie::Update()
{
	Player* p = GetBase<Player>();
	//モーションが死んだらプレイヤー負けStateにする
	if (p->playerCom.anim->IsFinish()) {
		p->playerCom.stateManager->SetNoStateChange(false);
		p->playerCom.gameManager->ChangeState(GameControler::GameState::LOSE);
		//p->playerCom.stateManager->ChangeState(StateID::PLAYER_ATTACK1_S);
	}
	else {
		//死んだ瞬間はスロー再生をさせる
		if (slowTime <= 0.0f) {
			Time::ChangeDeltaRate(1.0f);
			p->playerCom.camera->CameraShakeStop();
			p->playerCom.shaker->ShakeFinish();
		}
		else {
			slowTime -= Time::DeltaTime();
		}
	}
}

void PlayerDie::Draw()
{
}

void PlayerDie::Start()
{
	Player* p	= GetBase<Player>();
	PlayerStateBase::Start();
	Time::ChangeDeltaRate(0.1f);
	slowTime	= 0.5f;
	p->playerCom.camera->CameraShake(VOne * 10.0f, Shaker::MIX_SHAKE, false, -1.0f/*指定があるまでshake*/);
	p->playerCom.shaker->ShakeStart(VOne * 10.0f, Shaker::MIX_SHAKE, false, -1.0f/*指定があるまでshake*/);
	//one			= true;
	p->obj->Component()->RemoveAllComponent<SphereCollider>();
	SoundManager::GetInstance()->FeedInOut(Sound_ID::PLAY_BGM, 1.0f/*フェードイン時間*/);
}

void PlayerDie::Finish()
{
}
