#include "playerWalk.h"
#include "playerStateManager.h"
#include "../../Component/Physics/Physics.h"
#include "../../Component/Transform/Transform.h"
#include "../../Common/InputManager/PadInput.h"
#include "../../Common/Sound/SoundManager.h"
#include "../player.h"
#include "../../Component/Animator/Animator.h"

PlayerWalk::PlayerWalk()
{

	string = string = Function::GetClassNameC<PlayerWalk>();;
	animId = ID::P_ANIM_RUN;;
	//id = ID::P_ANIM_RUN;
	avoidStart = false;
	sound = false;
	firstOnes = false;
	secondOnes = false;
}

PlayerWalk::~PlayerWalk()
{

}

void PlayerWalk::Update()
{
	Player* p = GetBase<Player>();
	if (CheckHitKey(KEY_INPUT_1)) {
		p->playerCom.stateManager->ChangeState(StateID::PLAYER_WAIT_S);
	}
	DefalutWalk();

	
	//‘«‰¹–Â‚ç‚·ƒvƒƒOƒ‰ƒ€
	constexpr float FIRST_FOOT_SE = 5.0f;
	constexpr float FIRST_FOOT_SE_FINISH = 6.0f;
	if (p->playerCom.anim->GetCurrentFrame() >= FIRST_FOOT_SE && p->playerCom.anim->GetCurrentFrame() <= FIRST_FOOT_SE_FINISH) {
		if (firstOnes) {
			sound = true;
		}
		firstOnes = false;
	} 
	constexpr float SECOND_FOOT_SE = 17.0f;
	constexpr float SECOND_FOOT_SE_FINISH = 18.0f;
	if (p->playerCom.anim->GetCurrentFrame() >= SECOND_FOOT_SE && p->playerCom.anim->GetCurrentFrame() <= SECOND_FOOT_SE_FINISH) {
		if (secondOnes) {
			sound = true;
		}
		secondOnes = false;
	}
	if (sound) {
		//Žü”g”‚ð•Ï‚¦‚é
		const int RAMDOM_FREQUENCY = 10000;
		const int BASE_FREQUENCY = 60000;
		SoundManager::GetInstance()->PlayRamdomChangeFrequencySe(Sound_ID::PLAYER_WALK1,RAMDOM_FREQUENCY,BASE_FREQUENCY);
		sound = false;
	}
	constexpr float RESET_FOOT_SE_FRAME = 19.0f;
	if (p->playerCom.anim->GetCurrentFrame() >= RESET_FOOT_SE_FRAME) {
		firstOnes = true;
		secondOnes = true;
	}
}

void PlayerWalk::Draw()
{
	StateBase::Draw();
}

void PlayerWalk::Start()
{
	Player* p = GetBase<Player>();
	if (p->turn) {
		blendSpeed = 0.3f;
		p->turn = false;
	}
	else {
		blendSpeed = 0.2f;
	}
	
	PlayerStateBase::Start();
	p->playerCom.physics->SetFirction(PlayerInformation::BASE_INTERIA);
	p->attackTargetTrans = Transform();
	
}

void PlayerWalk::Finish()
{
	Player* p = GetBase<Player>();
	p->playerCom.anim->SetPlaySpeed(1.0f);
}
