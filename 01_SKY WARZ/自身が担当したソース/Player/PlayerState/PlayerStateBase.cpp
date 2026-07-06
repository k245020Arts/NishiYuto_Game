#include "playerStateBase.h"
#include "playerStateManager.h"
#include "../../Component/Animator/Animator.h"
#include "../Player.h"
#include "../../Component/Collider/SphereCollider.h"
#include "../../Common/Debug/Debug.h"
#include "../../Common/Sound/SoundManager.h"
#include "../../Common/InputManager/PadInput.h"
#include "../../Common/InputManager/InputManager.h"

PlayerStateBase::PlayerStateBase()
{
	firstColl		= false;
	nextAttack		= false;
	noStateChange	= false;
	animId = ID::P_ANIM_IDOL;
	blendSpeed = 0.1f;
}

PlayerStateBase::~PlayerStateBase()
{
}

void PlayerStateBase::Update()
{
}

void PlayerStateBase::Draw()
{

}

void PlayerStateBase::AvoidStart()
{
	Player* p		= GetBase<Player>();
	p->playerCom.stateManager->ChangeState(StateID::PLAYER_AVOID_S);
	noStateChange	= false;
}

void PlayerStateBase::Start() {
	Player* p		= GetBase<Player>();
	p->playerCom.anim->Play(animId,blendSpeed);
	noStateChange	= false;
	//Debug::DebugLog(string);
}

void PlayerStateBase::DefalutWalk()
{
	Player* p = GetBase<Player>();
	//ˆÚ“®ˆ—
	constexpr float BASE_SPEED = 20000.0f;
	constexpr float MAX_SPEED = 3200.0f;
	p->playerCom.player->Move(BASE_SPEED * Time::DeltaTimeRate(), MAX_SPEED);
	if (InputManager::GetInstance()->KeyInputDown("avoid")) {
		p->playerCom.player->AvoidReady();
	}
	if (InputManager::GetInstance()->KeyInputDown("attack")) {
		p->playerCom.stateManager->ChangeState(StateID::PLAYER_ATTACK1_S);
	}
	if (InputManager::GetInstance()->KeyInputDown("heavyAttack")) {
		p->playerCom.stateManager->ChangeState(StateID::PLAYER_HEAVY_CHARGE_S);
	}
	if (InputManager::GetInstance()->KeyInputDown("SpecialAttack")) {
		if (p->CanSpecialAttack()) {
			p->playerCom.stateManager->ChangeState(StateID::PLAYER_SPECIAL_ATTACK_S);
			p->specialAttackBar = 0.0f;
		}
		
	}
}